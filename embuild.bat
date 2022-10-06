mkdir .\build
mkdir .\build\html
emcc Dilgine\Dilgine\Scripts\main.cpp Dilgine\Dilgine\Scripts\Core\System\System_Emscripten.cpp Dilgine\Dilgine\Scripts\Game\GameObject.cpp Dilgine\Dilgine\Scripts\Game\Components\CollisionColorChanger.cpp Dilgine\Dilgine\Scripts\Game\Components\PlayerController.cpp Dilgine\Dilgine\Scripts\Game\Components\RectangleCollider.cpp Dilgine\Dilgine\Scripts\Game\Components\RectangleRenderer.cpp Dilgine\Dilgine\Scripts\Game\Components\Transform.cpp Dilgine\Dilgine\Scripts\Core\World.cpp -sUSE_SDL=2 -sLLD_REPORT_UNDEFINED -IDilgine\Dilgine\Scripts\Core -IDilgine\Dilgine\Scripts\Core\System -IDilgine\Dilgine\Scripts\Game -IDilgine\Dilgine\Scripts\Game\Components -IDilgine\Dilgine\Scripts\Core\World -o .\build\html\index.html