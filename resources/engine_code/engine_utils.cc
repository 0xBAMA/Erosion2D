#include "engine.h"
// This contains the lower level code

//TinyOBJLoader - This has to be included in a .cc file, so it's here for right now
#define TINYOBJLOADER_IMPLEMENTATION
// #define TINYOBJLOADER_USE_DOUBLE
#include "../TinyOBJLoader/tiny_obj_loader.h"


void engine::create_window()
{
    if(SDL_Init( SDL_INIT_EVERYTHING ) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8);

    // this is how you query the screen resolution
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    // pulling these out because I'm going to try to span the whole screen with
    // the window, in a way that's flexible on different resolution screens
    // int total_screen_width = dm.w;
    // int total_screen_height = dm.h;

    cout << "creating window...";

    // window = SDL_CreateWindow( "OpenGL Window", 0, 0, total_screen_width, total_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_BORDERLESS );
    window = SDL_CreateWindow( "OpenGL Window", 0, 0, HEIGHT, WIDTH, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE );
    SDL_ShowWindow(window);

    cout << "done." << endl;


    cout << "setting up OpenGL context...";
    // OpenGL 4.3 + GLSL version 430
    const char* glsl_version = "#version 430";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    GLcontext = SDL_GL_CreateContext( window );

    SDL_GL_MakeCurrent(window, GLcontext);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    // SDL_GL_SetSwapInterval(0); // explicitly disable vsync

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);

    glPointSize(3.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // void cast prevents unused variable warning

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, GLcontext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    clear_color = ImVec4(75.0f/255.0f, 75.0f/255.0f, 75.0f/255.0f, 0.5f); // initial value for clear color

    // really excited by the fact imgui has an hsv picker to set this
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( window );

    cout << "done." << endl;

    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text]                   = ImVec4(0.64f, 0.37f, 0.37f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.49f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.17f, 0.00f, 0.00f, 0.94f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.18f, 0.00f, 0.00f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.35f, 0.00f, 0.03f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void engine::gl_setup()
{
    // some info on your current platform
    const GLubyte *renderer = glGetString( GL_RENDERER ); // get renderer string
    const GLubyte *version = glGetString( GL_VERSION );  // version as a string
    printf( "Renderer: %s\n", renderer );
    printf( "OpenGL version supported %s\n\n\n", version );



    // create the shader for the triangles to cover the screen
    display_shader = Shader("resources/engine_code/shaders/blit.vs.glsl", "resources/engine_code/shaders/blit.fs.glsl").Program;

    // set up the points for the display
    //  A---------------B
    //  |          .    |
    //  |       .       |
    //  |    .          |
    //  |               |
    //  C---------------D

    // diagonal runs from C to B
    //  A is -1, 1
    //  B is  1, 1
    //  C is -1,-1
    //  D is  1,-1
    std::vector<glm::vec3> points;

    points.clear();
    points.push_back(glm::vec3(-1, 1, 0.5));  //A
    points.push_back(glm::vec3(-1,-1, 0.5));  //C
    points.push_back(glm::vec3( 1, 1, 0.5));  //B

    points.push_back(glm::vec3( 1, 1, 0.5));  //B
    points.push_back(glm::vec3(-1,-1, 0.5));  //C
    points.push_back(glm::vec3( 1,-1, 0.5));  //D

    // vao, vbo
    cout << "  setting up vao, vbo for display geometry...........";
    glGenVertexArrays( 1, &display_vao );
    glBindVertexArray( display_vao );

    glGenBuffers( 1, &display_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, display_vbo );
    cout << "done." << endl;

    // buffer the data
    cout << "  buffering vertex data..............................";
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * points.size(), &points[0]);
    cout << "done." << endl;

    // set up attributes
    cout << "  setting up attributes in display shader............";
    GLuint points_attrib = glGetAttribLocation(display_shader, "vPosition");
    glEnableVertexAttribArray(points_attrib);
    glVertexAttribPointer(points_attrib, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) (static_cast<const char*>(0) + (0)));
    cout << "done." << endl;

    // replace this with real image data
    // image_data.resize(WIDTH*HEIGHT*4);

    // fill with random values
    // std::default_random_engine gen;
    // std::uniform_int_distribution<unsigned char> dist(0,255);
    // for(auto it = image_data.begin(); it != image_data.end(); it++)
    //     *it = ((it-image_data.begin()) % 4 == 3) ? 255 : dist(gen); // alpha channels get 255, other colors get random

    
    // create the image textures
    glGenTextures(1, &display_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, display_texture);

    // texture parameters
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // generate a heightmap to work with
    generate_heightmap_diamond_square();

    // dump model to image
    dump_model("initial.png");
}

void engine::generate_heightmap_diamond_square()
{
    long unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine engine{seed};
    std::uniform_real_distribution<float> distribution{0, 1};
    
#ifdef TILE
    constexpr auto size = DIM;
#else
    constexpr auto size = DIM + 1; // for no_wrap
#endif

    constexpr auto edge = size - 1;

    uint16_t map[size][size] = {{0}};
    map[0][0] = map[edge][0] = map[0][edge] = map[edge][edge] = std::numeric_limits<uint16_t>::max()/2;

#ifdef TILE
    heightfield::diamond_square_wrap
#else
    heightfield::diamond_square_no_wrap
#endif
        (size,
        // random
        [&engine, &distribution](float range)
        {
            return distribution(engine) * range;
        },
        // variance
        [](int level) -> float
        {
            return static_cast<float>(std::numeric_limits<uint16_t>::max()/2) * std::pow(0.5f, level);
            // return static_cast<float>(std::numeric_limits<uint16_t>::max()/1.6) * std::pow(0.5f, level);
        },
        // at
        [&map](int x, int y) -> uint16_t&
        {
            return map[y][x];
        }
    );

    for(int x = 0; x < DIM; x++)
        for(int y = 0; y < DIM; y++)
            model[x][y] = map[x][y]/static_cast<float>(std::numeric_limits<uint16_t>::max());

    send_model_to_GPU();
}

void engine::send_model_to_GPU()
{
    // image_data.resize(0);
    std::vector<unsigned char> image_data;
    
    for(int x = 0; x < DIM; x++)
    {
        for(int y = 0; y < DIM; y++)
        {
            image_data.push_back(static_cast<unsigned char>(model[x][y]*255));
            image_data.push_back(static_cast<unsigned char>(model[x][y]*255));
            image_data.push_back(static_cast<unsigned char>(model[x][y]*255));
            // image_data.push_back(static_cast<unsigned char>(surface_normal(x,y).x*255));
            // image_data.push_back(static_cast<unsigned char>(surface_normal(x,y).y*255));
            // image_data.push_back(static_cast<unsigned char>(surface_normal(x,y).z*255));
            image_data.push_back(255);
        }
    }

    // buffer the image data to the GPU
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, display_texture);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8UI, DIM, DIM, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &image_data[0]);
    glBindImageTexture(0, display_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8UI);
}


void engine::dump_model(std::string filename)
{
    std::vector<unsigned char> image_data;
    
    for(int x = 0; x < DIM; x++)
    {
        for(int y = 0; y < DIM; y++)
        {
            image_data.push_back(static_cast<unsigned char>(model[x][y]*255));
            image_data.push_back(static_cast<unsigned char>(model[x][y]*255));
            image_data.push_back(static_cast<unsigned char>(model[x][y]*255));
            // image_data.push_back(static_cast<unsigned char>(surface_normal(x,y).x*255));
            // image_data.push_back(static_cast<unsigned char>(surface_normal(x,y).y*255));
            // image_data.push_back(static_cast<unsigned char>(surface_normal(x,y).z*255));
            image_data.push_back(255);
        }
    }
    
    unsigned error = lodepng::encode(filename.c_str(), image_data, DIM, DIM);
    if(error) std::cout << "encode error during save(\" "+ filename +" \") " << error << ": " << lodepng_error_text(error) << std::endl; 
}



glm::vec3 engine::surface_normal(int i, int j)
{
    float scale = 60.0;
    
    glm::vec3 n = glm::vec3(0.15) * glm::normalize(glm::vec3(scale*(model[i][j]-model[i+1][j]), 1.0, 0.0));   // Positive X
    n += glm::vec3(0.15) * glm::normalize(glm::vec3(scale*(model[i-1][j]-model[i][j]), 1.0, 0.0));           // Negative X
    n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale*(model[i][j]-model[i][j+1])));          // Positive Y
    n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale*(model[i][j-1]-model[i][j])));         // Negative Y

    // diagonals 
    n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(model[i][j]-model[i+1][j+1])/sqrt(2), sqrt(2), scale*(model[i][j]-model[i+1][j+1])/sqrt(2)));
    n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(model[i][j]-model[i+1][j-1])/sqrt(2), sqrt(2), scale*(model[i][j]-model[i+1][j-1])/sqrt(2)));
    n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(model[i][j]-model[i-1][j+1])/sqrt(2), sqrt(2), scale*(model[i][j]-model[i-1][j+1])/sqrt(2)));
    n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(model[i][j]-model[i-1][j-1])/sqrt(2), sqrt(2), scale*(model[i][j]-model[i-1][j-1])/sqrt(2)));

    return n;
}

void engine::erode(int steps)
{
    std::default_random_engine gen;
    std::uniform_int_distribution<int> dist(0,DIM);
    
    // run the simulation for the specified number of steps
    for(int i = 0; i < steps; i++)
    {
        //spawn a new particle at a random position
        particle p;
        p.position = glm::vec2(dist(gen), dist(gen));
        
        //while the droplet exists (drop volume > 0)
        while(p.volume > 0)
        {
            glm::ivec2 initial_position = p.position;    // cache the initial position
            glm::vec3 normal = surface_normal(initial_position.x, initial_position.y);

            // newton's second law to calculate acceleration
            p.speed += dt*glm::vec2(normal.x, normal.z)/(p.volume*density); // F = MA, A = F/M

            // update position based on new speed
            p.position += dt*p.speed;
            
            // friction factor to attenuate speed
            p.speed *= (1.0-dt*friction);

            // // wrap if out of bounds (mod logic)
            // particle_wrap(p);
            // if(glm::any(glm::isnan(p.position)))
            //     break;

            // thought I was clever, just discard if out of bounds
            if(!glm::all(glm::greaterThanEqual(p.position, glm::vec2(0))) ||
               !glm::all(glm::lessThan(p.position, glm::vec2(DIM)))) break; 

            // sediment capacity
            glm::ivec2 ref = glm::ivec2(p.position.x, p.position.y);
            float max_sediment = p.volume*glm::length(p.speed)*(model[initial_position.x][initial_position.y]-model[ref.x][ref.y]);
            max_sediment = std::max(max_sediment, 0.0f); // don't want negative values here
            float sediment_diff = max_sediment - p.sediment_fraction;
            
            // update sediment content, deposit on the heightmap
            p.sediment_fraction += dt*deposition_rate*sediment_diff;
            model[initial_position.x][initial_position.y] -= dt*p.volume*deposition_rate*sediment_diff;
            
            // evaporate the droplet
            p.volume *= (1.0-dt*evaporation_rate);
        }
    }
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void engine::draw_everything()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io; // void cast prevents unused variable warning
    //get the screen dimensions and pass in as uniforms


    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);   // from hsv picker
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     // clear the background


    // update the model
    erode(5000);
    
    // buffer resulting image to GPU
    send_model_to_GPU();
    
    // texture display
    glUseProgram(display_shader);
    glBindVertexArray( display_vao );
    glBindBuffer( GL_ARRAY_BUFFER, display_vbo );

    glDrawArrays( GL_TRIANGLES, 0, 6 );


    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    // show the demo window
    // static bool show_demo_window = true;
    // if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // do my own window
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,385));
    ImGui::Begin("Controls", NULL, 0);

    //do the other widgets
    HelpMarker("shut up, compiler");


    ImGui::End();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());   // put imgui data into the framebuffer

    SDL_GL_SwapWindow(window);  // swap the double buffers

    // handle events

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            pquit = true;

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            pquit = true;

        if ((event.type == SDL_KEYUP  && event.key.keysym.sym == SDLK_ESCAPE) || (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_X1)) //x1 is browser back on the mouse
            pquit = true;
    }
}


void engine::quit()
{
    // dump model to image
    dump_model("eroded.png");

    //shutdown everything
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    //destroy window
    SDL_GL_DeleteContext(GLcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    cout << "goodbye." << endl;
}
