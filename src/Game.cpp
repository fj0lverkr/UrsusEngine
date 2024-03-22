#include <sstream>
#include "Game.hpp"
#include "TiledMapManager.hpp"
#include "Collision.hpp"
#include "ECS/Components.hpp"
#include "ECS/Animation.hpp"

SDL_Renderer *Game::renderer = nullptr;

bool Game::isRunning = false;
bool Game::isDebug = false;
int Game::scaleFactor = 0;
Camera2D Game::camera;
SDL_Event Game::event;

Manager manager;
AssetManager* Game::assets = new AssetManager(&manager);
KeyboardController Game::keyboardController;

UILabel* Game::debugLabel = nullptr;
std::vector<Entity*> ysortables;

auto& player(manager.addEntity());
auto& debugHud(manager.addEntity());

TiledMapManager mapManager;

Game::Game()
{
    window = {};
    windowHeight = windowWidth = 0;
}

Game::~Game()
{
}

bool Game::ysortEntities(Entity *e1, Entity *e2)
{
    // Function to sort the vector containing the y-sortable entities
    float e1MinY, e1MaxY, e2MinY, e2MaxY;
    e1MinY = e1MaxY =  e2MinY = e2MaxY = -1.0f;

    if (e1->hasComponent<AnchorComponent>()) {
        auto& anchor = e1->GetComponent<AnchorComponent>();
        e1MinY = e1MaxY = anchor.transform->position.y;
    }
    else if (e1->hasComponent<ColliderComponent>()) {
        auto& collider = e1->GetComponent<ColliderComponent>();
        if (collider.getType() == ColliderType::AABB)
        {
            e1MinY = collider.transform->position.y;
            e1MaxY = e1MinY + collider.transform->height;
        }
        else {
            std::vector<SDL_FPoint> colliderPoints = collider.colliderPoints;
            e1MinY = e1MaxY = colliderPoints[0].y;
            for (auto& p : colliderPoints)
            {
                e1MinY = p.y < e1MinY ? p.y : e1MinY;
                e1MaxY = p.y > e1MaxY ? p.y : e1MaxY;
            }
        }
    }

    if (e2->hasComponent<AnchorComponent>()) {
        auto& anchor = e2->GetComponent<AnchorComponent>();
        e2MinY = e2MaxY = anchor.transform->position.y;
    }
    else if (e2->hasComponent<ColliderComponent>()) {
        auto& collider = e2->GetComponent<ColliderComponent>();
        if (collider.getType() == ColliderType::AABB)
        {
            e2MinY = collider.transform->position.y;
            e2MaxY = e2MinY + collider.transform->height;
        }
        else {
            std::vector<SDL_FPoint> colliderPoints = collider.colliderPoints;
            e2MinY = e2MaxY = colliderPoints[0].y - Game::camera.GetViewFinder().y;
            for (auto& p : colliderPoints)
            {
                e2MinY = p.y - Game::camera.GetViewFinder().y < e2MinY ? p.y - Game::camera.GetViewFinder().y : e2MinY;
                e2MaxY = p.y - Game::camera.GetViewFinder().y > e2MaxY ? p.y - Game::camera.GetViewFinder().y : e2MaxY;
            }
        }
    }

    if(e1MinY >= e2MaxY && e1MaxY >= e2MinY)
        return true;
    if (e1MinY < e2MaxY || e1MaxY < e2MinY)
        return false;
}

void Game::init(const char *title, int xpos, int ypos, int width, int height, int scaleFactor, bool fullscreen, SDL_Color rendererColor, bool debug)
{
    isDebug = debug;

    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window)
        {
            windowWidth = width;
            windowHeight = height;
            this->scaleFactor = scaleFactor;
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer)
            {
                SDL_SetRenderDrawColor(renderer, rendererColor.r, rendererColor.g, rendererColor.b, rendererColor.a);
                isRunning = true;
                Game::camera = {0.0f, 0.0f, width, height};
                if (TTF_Init() == -1)
                {
                    std::cout << "Error initialtizing SDL_ttf, font rendering might be disabled." << std::endl;
                }
            }
        }
    }
    else
    {
        isRunning = false;
    }
    Game::assets->AddTiledMaps("assets/tiled");
    //TODO move this to the AssetManager and let it load all png files in a given folder.
    Game::assets->AddTexture("PlayerSprite", "assets/sprites/player_anim.png");
    Game::assets->AddTexture("Placeholder", "assets/placeholder.png");
    Game::assets->AddFont("Swansea_16", "assets/fonts/Swansea.ttf", 16);

    mapManager.LoadMap("testmap", scaleFactor, true);

    player.AddGroup(groupPlayers);
    debugHud.AddGroup(groupUI);

    // setup player sprite animation
    std::map<const char*, Animation> playerAnimations;
    Animation idle = Animation(0, 9, 100);
    Animation walkSide = Animation(1, 7, 100);
    Animation walkFront = Animation(2, 7, 100);
    playerAnimations.emplace("Idle", idle);
    playerAnimations.emplace("WalkSide", walkSide);
    playerAnimations.emplace("WalkFront", walkFront);

    player.addComponent<TransformComponent>(static_cast<float>(windowWidth) / 2, static_cast<float>(windowHeight) / 2, 32.0f, 32.0f, scaleFactor);
    player.addComponent<SpriteComponent>("PlayerSprite", playerAnimations, "Idle");
    player.addComponent<ColliderComponent>("player", ColliderType::AABB);
    player.addComponent<AnchorComponent>("playerAnchor", AnchorComponent::AnchorBottom, 0.0f, 2.0f, isDebug);
    player.addComponent<KeyboardController>();

    keyboardController = player.GetComponent<KeyboardController>();

    debugHud.addComponent<TransformComponent>(10.0f, 10.0f);
    debugHud.addComponent<UILabel>("Test", "Swansea_16");

    debugLabel = &debugHud.GetComponent<UILabel>();
}

auto& mapTiles(manager.getGroup(Game::groupMapTiles));
auto& mapObjects(manager.getGroup(Game::groupMapObjects));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& uiElements(manager.getGroup(Game::groupUI));

void Game::update(int fps) const
{
    std::stringstream ss;

    if (isDebug)
    {
        ss << "Player: " << player.GetComponent<TransformComponent>().position << " | FPS: " << fps;
        debugLabel->setLabelText(ss.str());
    }

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            Game::isRunning = false;
        }
        if (keyboardController.HandleEvent(&event))
        {
            continue;
        }
    }

    Vector2D playerPos = player.GetComponent<TransformComponent>().position;
    manager.refresh();
    manager.update();

    for (auto& c : colliders) {
        if (Collision::AABB(c->GetComponent<ColliderComponent>(), player.GetComponent<AnchorComponent>()))
        {
            player.GetComponent<TransformComponent>().position = playerPos;
        }
        if (Collision::PointCollision(c->GetComponent<ColliderComponent>(), player.GetComponent<AnchorComponent>()))
        {
            player.GetComponent<TransformComponent>().position = playerPos;
        }
    }

    camera.follow(player.GetComponent<TransformComponent>().position, windowWidth, windowHeight);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    ysortables.clear();
    for (auto &t : mapTiles)
    {
        t->draw();
    }
    for (auto &o : mapObjects)
    {
        ysortables.emplace_back(o);
    }
    for (auto &p : players)
    {
        ysortables.emplace_back(p);
    }
    for (auto &e : enemies)
    {
        ysortables.emplace_back(e);
    }

    // TODO sort the ysortables by the y values of their colliders (player anchor, enemy achor, object collider) and draw them in that order
    std::sort(ysortables.begin(), ysortables.end(), ysortEntities);
    for (auto &d : ysortables)
    {
        d->draw();
    }

    if (isDebug)
    {
        for (auto &c : colliders)
        {
            c->draw();
        }
    }
    for (auto &ui : uiElements)
    {
        ui->draw();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
