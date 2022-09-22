mkdir .\build
mkdir .\build\html
emcc Dilgine/Dilgine/Source/main.cpp -sUSE_SDL=2 -IDilgine\Dilgine\Header -o .\build\html\index.html