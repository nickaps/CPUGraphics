# CPUGraphics
### Created by Nick Chapman
### Project started on 4/23/2026

	A simple graphics rasterizer (sans hardware acceleration) that
	uses SDL and is written in C++. The goal is to be able to render
	any mesh with UV data and textures.

	It will be a convention that methods that I write for the engine
	that handle behavior (mostly utilizing SDL) will begin with "e..."

	e.g.	eInitializeWindow();
			eRenderLoop();
			eInitialize();
			eDestroy();
			...
			
If you are trying to launch from an IDE like Visual Studio, it's possible
that your IDE is opening from a path different that where the binary
lives. This will cause your meshes to not load and then for your
instance to crash. Try using the build.bat file or launching the .exe in
the binary folder.