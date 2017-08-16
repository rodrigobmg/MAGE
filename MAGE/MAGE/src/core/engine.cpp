//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "core\engine.hpp"
#include "core\version.hpp"
#include "rendering\display_configurator.hpp"
#include "logging\error.hpp"
#include "logging\exception.hpp"
#include "logging\logging.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// Engine
	//-------------------------------------------------------------------------
	
	Engine *Engine::s_engine = nullptr;

	Engine::Engine(const EngineSetup &setup) 
		: Loadable(), m_main_window(), m_deactive(false), 
		m_renderer(), m_mode_switch(false),
		m_input_manager(), m_resource_manager(),
		m_scene_manager(), m_timer(MakeUnique< Timer >()),
		m_engine_stats(MakeUnique< EngineStatistics >()) {

		s_engine = this;

		// Initialize the systems of this engine.
		InitializeSystems(setup);

		if (!IsLoaded()) {
			return;
		}

		// Initialize the first scene.
		SetScene(setup.CreateScene());
	}

	Engine::Engine(Engine &&engine) = default;

	Engine::~Engine() {

		// Uninitialize the COM.
		CoUninitialize();

		s_engine = nullptr;
	}

	void Engine::InitializeSystems(const EngineSetup &setup) {

		// Initialize a console.
		InitializeConsole();
		PrintConsoleHeader();

		// Enumerate the devices.
		UniquePtr< DisplayConfigurator > display_configurator(MakeUnique< DisplayConfigurator >());
		const HRESULT result_configure = display_configurator->Configure();
		if (FAILED(result_configure)) {
			Error("Display configuration failed: %ld", result_configure);
			return;
		}

		const DisplayConfiguration *display_configuration = display_configurator->GetDisplayConfiguration();
		
		// Initialize the window System.
		const uint32_t width  = display_configuration->GetDisplayWidth();
		const uint32_t height = display_configuration->GetDisplayHeight();
		m_main_window         = MakeUnique< MainWindow >(setup.GetApplicationHinstance(), setup.GetApplicationName(), width, height);
		// Initialize the rendering system.
		m_renderer            = MakeUnique< Renderer >(m_main_window->GetHandle(), display_configuration);
		// Initialize the input system.
		m_input_manager       = MakeUnique< InputManager >(m_main_window->GetHandle());
		// Initialize the resource system.
		m_resource_manager    = MakeUnique< ResourceManager >();
		// Initialize the scene system.
		m_scene_manager       = MakeUnique< SceneManager >();

		// Initializes the COM library for use by the calling thread 
		// and sets the thread's concurrency model to multithreaded concurrency.
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		SetLoaded();
	}

	void Engine::SetDeactiveFlag(bool deactive) {
		m_deactive = deactive;
		
		if (m_deactive) {
			m_timer->Stop();
		}
		else {
			m_timer->Resume();
		}
	}

	void Engine::SetScene(UniquePtr< Scene > &&scene) {
		m_timer->Stop();
		
		m_scene_manager->SetScene(std::move(scene));
		
		m_timer->Restart();
	}

	int Engine::Run(int nCmdShow) {
		if (!IsLoaded()) {
			Error("Game loop can not start because the engine is not loaded.");
			return 0;
		}
		if (m_scene_manager->IsFinished()) {
			return 0;
		}

		m_main_window->Show(nCmdShow);
		// Handle startup in fullscreen mode.
		m_renderer->SetInitialMode();

		m_timer->Restart();

		// Enter the message loop.
		MSG msg;
		SecureZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT) {
			
			// Retrieves messages for any window that belongs to the current thread
			// without performing range filtering. Furthermore messages are removed
			// after processing.
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				// Translates virtual-key messages into character messages.
				TranslateMessage(&msg);
				// Dispatches a message to a window procedure.
				DispatchMessage(&msg);
				continue;
			}

			if (m_deactive) {
				continue;
			}

			// Update the input manager.
			m_input_manager->Update();
			// Handle forced exit.
			if (m_input_manager->GetKeyboard()->GetKeyPress(DIK_F1)) {
				PostQuitMessage(0);
				continue;
			}

			// Handle switch between full screen and windowed mode.
			const bool lost_mode = m_renderer->LostMode();
			if (m_mode_switch || lost_mode) {
				m_renderer->SwitchMode(!lost_mode);
				m_mode_switch = false;
				continue;
			}

			// Calculate the elapsed time.
			const double delta_time = m_timer->GetDeltaTime();

			// Update the current scene.
			m_scene_manager->Update(delta_time);
			if (m_scene_manager->IsFinished()) {
				PostQuitMessage(0);
				continue;
			}
				
			// Render the current scene.
			m_engine_stats->PrepareRendering();
			m_renderer->BeginFrame();
			m_renderer->PrepareRendering3D();
			m_scene_manager->Render3D();
			//m_scene_manager->RenderBoundingBoxes();
			m_renderer->PrepareRendering2D();
			m_scene_manager->Render2D();
			m_renderer->EndFrame();
		}

		return static_cast< int >(msg.wParam);
	}
}
