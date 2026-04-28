#define SDL_MAIN_HANDLED
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

#include "GameState.h"
#include "UIRenderer.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "Shader.h"
#include "MarketMap.h"
#include "AntoniAI.h"
#include "CustomerAI.h"
#include "TaskSystem.h"
#include "EventManager.h"
#include "ParticleSystem.h"
#include "PostProcess.h"
#include "SaveLoad.h"
#include "GlobalContext.h"
#include "AntiCheat.h"

static constexpr int   W            = 1280;
static constexpr int   H            = 720;
static constexpr float PLAYER_SPEED = 3.8f;
static constexpr float FEAR_DRAIN   = 1.5f;
static constexpr float FEAR_REGEN   = 0.3f;

static float sceneTimer = 0.0f;

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow("THE SUPERMARKET", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
    SDL_GL_MakeCurrent(window, glContext);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto& state  = GameStateMachine::Get().Data();
    auto& audio  = AudioManager::Get();
    auto& input  = InputManager::Get();
    auto& ac     = SimpleAntiCheat::Get();
    
    GlobalContext::Get().Init();
    audio.Init();
    input.Init();
    ac.Init();
    SaveLoad::LoadSettings(state);

    UIRenderer    ui;    ui.Init(W, H);
    PostProcess   post;  post.Init(W, H);

    GlobalContext::Get().ui = &ui;
    GlobalContext::Get().post = &post;

    Camera*        cam       = nullptr;
    MarketMap*     market    = nullptr;
    AntoniAI*      antoni    = nullptr;
    CustomerAI*    customer1 = nullptr;
    CustomerAI*    customer2 = nullptr;
    TaskSystem*    tasks     = nullptr;
    EventManager*  events    = nullptr;
    ParticleSystem* particles= nullptr;
    Shader*        worldShader=nullptr;

    float prevTime = (float)SDL_GetTicks() / 1000.0f;
    bool quit = false;

    while (!quit) {
        float now = (float)SDL_GetTicks() / 1000.0f;
        float dt  = glm::clamp(now - prevTime, 0.0f, 0.1f);
        prevTime  = now;

        input.PollEvents(quit);
        auto& ctx = GlobalContext::Get();
        ac.Update(dt);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (input.IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
            if (state.currentScene == GameScene::GAMEPLAY_DAY1  ||
                state.currentScene == GameScene::GAMEPLAY_DAY5  ||
                state.currentScene == GameScene::GAMEPLAY_CHASE) {
                state.isPaused = !state.isPaused;
                input.SetCursorLocked(!state.isPaused);
                audio.PlaySFX(AudioManager::SFX_CLICK);
            } else if (state.currentScene == GameScene::SETTINGS) {
                GameStateMachine::Get().TransitionTo(GameScene::MAIN_MENU);
            }
        }

        sceneTimer += dt;

        switch (state.currentScene) {
        case GameScene::LOADING: {
            static float loadT = 0.0f;
            loadT += dt * 0.28f;
            state.loadProgress = glm::min(loadT, 1.0f);
            ui.DrawLoadingScreen(state.loadProgress, "Summoning Antoni from Aisle 7...");

            if (state.loadProgress >= 1.0f && !cam) {
                cam      = new Camera({0.f, 1.7f, 3.f});
                market   = new MarketMap();
                market->Init("assets/textures/world_skin.png",
                             "assets/textures/world_skin.png",
                             "assets/textures/shelf_tex.png");
                antoni   = new AntoniAI("assets/models/characters/entity_antoni.obj");
                customer1= new CustomerAI(CustomerType::STRANGE,
                             market->GetZoneCenter("AISLE_A"),
                             "assets/models/characters/entity_strange_customer.obj");
                customer2= new CustomerAI(CustomerType::STRANGE,
                             market->GetZoneCenter("CHECKOUT"),
                             "assets/models/characters/entity_strange_customer.obj");
                tasks    = new TaskSystem();
                tasks->InitDay1Tasks();
                events   = new EventManager();
                particles= new ParticleSystem(1024);
                
                auto& ctx = GlobalContext::Get();
                ctx.player->camera = Camera({0.f, 1.7f, 3.f});
                cam = &ctx.player->camera;
                ctx.market = market;
                ctx.antoni = antoni;
                ctx.tasks  = tasks;
                ctx.events = events;
                ctx.particles = particles;
                ctx.activeCustomers.push_back(customer1);
                ctx.activeCustomers.push_back(customer2);

                worldShader = new Shader(
                    "assets/shaders/world/world.vert",
                    "assets/shaders/world/world.frag");
                events->RegisterDay1Events(*antoni, *customer1, audio, GameStateMachine::Get());
                GameStateMachine::Get().TransitionTo(GameScene::MAIN_MENU);
                audio.PlayBGM(AudioManager::BGM_MENU);
            }
            break;
        }

        case GameScene::MAIN_MENU: {
            static int hovered = 0;
            int dx=0, dy=0;
            input.Navigate(dx, dy);
            if (dy != 0) {
                hovered = glm::clamp(hovered + dy, 0, 3);
                audio.PlaySFX(AudioManager::SFX_CLICK);
            }
            if (input.Confirm()) {
                audio.PlaySFX(AudioManager::SFX_CLICK);
                if (hovered == 0) {
                    GameStateMachine::Get().TransitionTo(GameScene::CUTSCENE_DAY1);
                    audio.StopBGM();
                    sceneTimer = 0.0f;
                } else if (hovered == 1 && SaveLoad::SaveExists()) {
                    SaveLoad::LoadGame(state, *ctx.player);
                    GameStateMachine::Get().TransitionTo(state.currentScene);
                } else if (hovered == 2) {
                    GameStateMachine::Get().TransitionTo(GameScene::SETTINGS);
                } else if (hovered == 3) {
                    quit = true;
                }
            }
            ui.DrawMainMenu(hovered);
            break;
        }

        case GameScene::SETTINGS:
            ui.DrawSettingsMenu(state);
            break;

        case GameScene::CUTSCENE_DAY1:
            ui.DrawCutsceneDay1(sceneTimer);
            if (sceneTimer >= 6.0f) {
                GameStateMachine::Get().TransitionTo(GameScene::GAMEPLAY_DAY1);
                input.SetCursorLocked(true);
                audio.PlayBGM(AudioManager::BGM_NORMAL_SHIFT, true, 0.35f);
                sceneTimer = 0.0f;
            }
            break;

        case GameScene::GAMEPLAY_DAY1:
            if (state.isPaused) {
                ui.DrawPauseMenu(0);
            } else {
                post.BeginCapture();
                glm::vec3 dir{0};
                float speed = PLAYER_SPEED;
                if (input.MoveForward())  dir += cam->Front();
                if (input.MoveBackward()) dir -= cam->Front();
                if (input.MoveLeft())     dir -= cam->Right();
                if (input.MoveRight())    dir += cam->Right();
                dir.y = 0;
                if (glm::length(dir) > 0.01f) dir = glm::normalize(dir);
                cam->Move(dir, speed, dt);
                cam->Update(input.MouseDelta().x, input.MouseDelta().y, state.mouseSensitivity);
                cam->Bob(glm::length(dir) > 0.01f, dt);

                glm::vec3 p = cam->Position();
                AABB playerBox = MakeAABB(p, {0.3f, 0.9f, 0.3f});
                for (auto& wall : market->GetColliders()) {
                    if (playerBox.Intersects(wall)) {
                        glm::vec3 mtv = AABB::Resolve(playerBox, wall);
                        cam->Move(mtv, 1.0f, 1.0f);
                    }
                }

                tasks->Update(cam->Position(), input.Interact());
                events->Update(dt);

                if (state.currentScene == GameScene::GAMEPLAY_DAY1 && glm::distance(cam->Position(), market->GetTVPos()) < 2.0f && input.Interact()) {
                    market->SetLightsOut(true);
                    audio.PlaySFX(AudioManager::SFX_CLICK);
                }

                if (state.antoniChasing && glm::distance(cam->Position(), market->GetFrozenSectionPos()) < 3.0f) {
                    market->OpenSecretDoor();
                }

                worldShader->Use();
                worldShader->SetMat4("view", cam->GetViewMatrix());
                worldShader->SetMat4("projection", cam->GetProjectionMatrix(W, H));
                worldShader->SetVec3("viewPos", cam->Position());
                worldShader->SetVec3("lightPos", {0,3,0});
                worldShader->SetFloat("time", now);
                worldShader->SetFloat("fearFactor", ctx.player->fearLevel / 100.0f);
                worldShader->SetFloat("chaseFactor", 0.0f);

                market->Draw(*worldShader, cam->GetViewMatrix(), cam->GetProjectionMatrix(W, H), now, ctx.player->fearLevel / 100.0f);
                customer1->Draw(*worldShader, cam->GetViewMatrix(), cam->GetProjectionMatrix(W, H));
                particles->Update(dt);
                particles->Draw(*worldShader, cam->GetViewMatrix(), cam->GetProjectionMatrix(W, H), cam->Right(), cam->Front());

                post.EndCapture();
                post.Render(ctx.player->fearLevel, now, false, false, false, 0.0f);

                bool nearTask = tasks->IsNearActiveTask(cam->Position());
                std::string taskDesc = tasks->GetActiveTask() ? tasks->GetActiveTask()->description : "Survive.";
                std::string hint = nearTask ? "[E] " + (tasks->GetActiveTask() ? tasks->GetActiveTask()->subText : "") : "";
                ui.DrawHUD(ctx.player->fearLevel, taskDesc, nearTask, hint, state.playTime, ctx.player->inventory.hasCarKeys, state.antoniChasing);
                state.playTime += dt;
            }
            break;

        case GameScene::CUTSCENE_POOP1:
            ui.DrawPoopEvent1(sceneTimer);
            if (sceneTimer >= 8.0f) {
                GameStateMachine::Get().TransitionTo(GameScene::CUTSCENE_DAY5);
                sceneTimer = 0.0f;
            }
            break;

        case GameScene::CUTSCENE_DAY5:
            ui.DrawCutsceneDay5(sceneTimer);
            if (sceneTimer >= 12.0f) {
                tasks->InitDay5Tasks();
                events->RegisterDay5Events(*antoni, *customer2, *market, audio, GameStateMachine::Get());
                GameStateMachine::Get().TransitionTo(GameScene::GAMEPLAY_DAY5);
                audio.PlayBGM(AudioManager::BGM_TENSION, true, 0.3f);
                input.SetCursorLocked(true);
                sceneTimer = 0.0f;
            }
            break;

        case GameScene::GAMEPLAY_DAY5:
            if (state.isPaused) { ui.DrawPauseMenu(0); }
            else {
                post.BeginCapture();
                glm::vec3 dir{0};
                if (input.MoveForward())  dir += cam->Front();
                if (input.MoveBackward()) dir -= cam->Front();
                if (input.MoveLeft())     dir -= cam->Right();
                if (input.MoveRight())    dir += cam->Right();
                dir.y = 0; if (glm::length(dir) > 0.01f) dir = glm::normalize(dir);
                cam->Move(dir, PLAYER_SPEED, dt);
                cam->Update(input.MouseDelta().x, input.MouseDelta().y, state.mouseSensitivity);
                tasks->Update(cam->Position(), input.Interact());
                events->Update(dt);
                worldShader->Use();
                market->Draw(*worldShader, cam->GetViewMatrix(), cam->GetProjectionMatrix(W, H), now, ctx.player->fearLevel / 100.0f);
                customer2->Draw(*worldShader, cam->GetViewMatrix(), cam->GetProjectionMatrix(W, H));
                particles->Update(dt);
                particles->Draw(*worldShader, cam->GetViewMatrix(), cam->GetProjectionMatrix(W, H), cam->Right(), cam->Front());
                post.EndCapture();
                post.Render(ctx.player->fearLevel, now, false, false, false, 0.0f);
                ui.DrawHUD(ctx.player->fearLevel, tasks->GetActiveTask() ? tasks->GetActiveTask()->description : "Survive.", tasks->IsNearActiveTask(cam->Position()), "", state.playTime, ctx.player->inventory.hasCarKeys, false);
                state.playTime += dt;
            }
            break;

        case GameScene::CUTSCENE_POOP2:
            ui.DrawPoopEvent2(sceneTimer);
            if (sceneTimer >= 6.0f) {
                GameStateMachine::Get().TransitionTo(GameScene::CUTSCENE_CHASE);
                sceneTimer = 0.0f;
            }
            break;

        case GameScene::CUTSCENE_CHASE:
            ui.DrawBabyOilCutscene(sceneTimer);
            if (sceneTimer >= 5.0f) {
                audio.StopBGM();
                audio.PlayBGM(AudioManager::BGM_CHASE, true, 0.9f);
                state.antoniChasing = true;
                input.SetCursorLocked(true);
                GameStateMachine::Get().TransitionTo(GameScene::GAMEPLAY_CHASE);
                sceneTimer = 0.0f;
            }
            break;

        case GameScene::GAMEPLAY_CHASE: {
            if (state.isPaused) { ui.DrawPauseMenu(0); }
            else {
                post.BeginCapture();
                glm::vec3 dir{0};
                float speed = PLAYER_SPEED * (input.Sprint() ? 1.8f : 1.0f);
                if (input.MoveForward())  dir += cam->Front();
                if (input.MoveBackward()) dir -= cam->Front();
                if (input.MoveLeft())     dir -= cam->Right();
                if (input.MoveRight())    dir += cam->Right();
                dir.y = 0; if (glm::length(dir) > 0.01f) dir = glm::normalize(dir);

                if (market->PlayerInOilPuddle(cam->Position()) && !ctx.player->isSlipping) {
                    ctx.player->isSlipping = true; ctx.player->slipTimer = 1.2f; audio.PlaySFX("assets/audio/sfx_slip.wav");
                }
                if (ctx.player->isSlipping) {
                    cam->ApplySlip(dt); ctx.player->slipTimer -= dt;
                    if (ctx.player->slipTimer <= 0.0f) { ctx.player->isSlipping = false; cam->StopSlip(); }
                } else { cam->Move(dir, speed, dt); }

                cam->Update(input.MouseDelta().x, input.MouseDelta().y, state.mouseSensitivity);
                cam->ApplyFearEffect(ctx.player->fearLevel, now);
                antoni->Update(dt, cam->Position(), now, *particles);

                float dist = antoni->DistanceTo(cam->Position());
                if (dist < 8.0f) ctx.player->fearLevel = glm::min(100, ctx.player->fearLevel + (int)(FEAR_DRAIN * dt * 60.0f));
                else ctx.player->fearLevel = glm::max(0, ctx.player->fearLevel - (int)(FEAR_REGEN * dt * 60.0f));

                if (!ctx.player->inventory.hasCarKeys && glm::distance(cam->Position(), market->GetCarKeysSpawn()) < 1.5f && input.Interact()) {
                    ctx.player->inventory.hasCarKeys = true; audio.PlaySFX("assets/audio/sfx_keys_found.wav");
                }
                if (ctx.player->inventory.hasCarKeys && glm::distance(cam->Position(), market->GetCarPos()) < 2.5f) {
                    audio.PlaySFX("assets/audio/sfx_car_start.wav"); audio.StopBGM();
                    GameStateMachine::Get().TransitionTo(GameScene::WIN_SCREEN); sceneTimer = 0.0f;
                }
                if (antoni->CaughtPlayer(cam->Position())) {
                    state.deathCount++; audio.StopBGM(); audio.PlaySFX("assets/audio/sfx_antoni_catch.wav");
                    GameStateMachine::Get().TransitionTo(GameScene::GAME_OVER); sceneTimer = 0.0f;
                }

                worldShader->Use();
                market->Draw(*worldShader, cam->GetViewMatrix(), cam->GetProjectionMatrix(W, H), now, ctx.player->fearLevel / 100.0f);
                antoni->Draw(*worldShader, cam->GetViewMatrix(), cam->GetProjectionMatrix(W, H));
                particles->Update(dt);
                particles->Draw(*worldShader, cam->GetViewMatrix(), cam->GetProjectionMatrix(W, H), cam->Right(), cam->Front());
                post.EndCapture();
                post.Render(ctx.player->fearLevel, now, true, ctx.player->isSlipping, false, 0.0f);
                ui.DrawHUD(ctx.player->fearLevel, "FIND YOUR CAR KEYS AND ESCAPE", false, "", state.playTime, ctx.player->inventory.hasCarKeys, true);
                state.playTime += dt;
            }
            break;
        }

        case GameScene::GAME_OVER:
            ui.DrawGameOver(state.deathCount, state.playTime);
            if (input.Confirm()) {
                ctx.player->fearLevel = 0; ctx.player->inventory.hasCarKeys = false; state.antoniChasing = false;
                antoni->Reset(); cam->Move(market->GetPlayerSpawn() - cam->Position(), 1.0f, 1.0f);
                GameStateMachine::Get().TransitionTo(GameScene::GAMEPLAY_CHASE);
                audio.PlayBGM(AudioManager::BGM_CHASE, true, 0.9f); sceneTimer = 0.0f;
            }
            break;

        case GameScene::WIN_SCREEN:
            ui.DrawWinScreen(state.playTime, state.deathCount);
            if (input.Confirm() && sceneTimer > 3.0f) {
                GameStateMachine::Get().TransitionTo(GameScene::CREDITS); sceneTimer = 0.0f;
            }
            break;

        case GameScene::CREDITS:
            ui.DrawCredits(sceneTimer * 35.0f);
            if (input.Confirm()) {
                GameStateMachine::Get().Reset(); GameStateMachine::Get().TransitionTo(GameScene::MAIN_MENU);
                audio.PlayBGM(AudioManager::BGM_MENU); sceneTimer = 0.0f;
            }
            break;

        default: break;
        }

        SDL_GL_SwapWindow(window);
    }

    audio.Shutdown();
    delete cam; delete market; delete antoni;
    delete customer1; delete customer2;
    delete tasks; delete events; delete particles; delete worldShader;
    ui.Shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
