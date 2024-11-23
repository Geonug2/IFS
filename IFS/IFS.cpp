#include <glad/glad.h> // Lisame GLADi päise OpenGL funktsioonide haldamiseks
#include <GLFW/glfw3.h> // Lisame GLFW päise akna haldamiseks ja sisendi töötlemiseks
#include <iostream> // Lisame iostreami standardse sisendi/väljundi jaoks

#define STB_IMAGE_IMPLEMENTATION // Määrame selle enne stb_image.h lisamist, et teostada teeki
#include "stb_image.h" // Lisame stb_image'i piltide laadimiseks

// Funktsioonide deklaratsioonid
void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Callback akna suuruse muutmiseks
void processInput(GLFWwindow* window); // Funktsioon sisendi töötlemiseks

// Vertex shaderi allikas
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // Positsiooni atribuut
"layout (location = 1) in vec3 aColor;\n" // Värvi atribuut
"layout (location = 2) in vec2 aTexCoord;\n" // Tekstuurikoordinaadi atribuut
"out vec3 ourColor;\n" // Väljundvärv fragment shaderisse
"out vec2 TexCoord;\n" // Väljundtekstuurikoordinaat fragment shaderisse
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n" // Määrame verteksi positsiooni
"    ourColor = aColor;\n" // Edastame värvi fragment shaderisse
"    TexCoord = aTexCoord;\n" // Edastame tekstuurikoordinaadid fragment shaderisse
"}\0"; // Nullterminaator stringi lõpus

// Fragment shaderi allikas
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n" // Väljundvärv
"in vec3 ourColor;\n" // Väljundvärv vertex shaderist
"in vec2 TexCoord;\n" // Väljundtekstuurikoordinaadid vertex shaderist
"uniform sampler2D ourTexture;\n" // Tekstuurisampler
"void main() {\n"
"    FragColor = texture(ourTexture, TexCoord);\n" // Proovime tekstuuri ja määrame fragmentide värvi
"}\0"; // Nullterminaator stringi lõpus

int main() {
    glfwInit(); // Algatame GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Määrame peamise OpenGL versiooni
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Määrame alamsisendi OpenGL versiooni
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Kasutame tuumprofiili

    // Loome akna ja selle OpenGL konteksti
    GLFWwindow* window = glfwCreateWindow(800, 600, "LeanOpenGL", NULL, NULL);
    if (window == NULL) { // Kontrollime, kas akna loomine ebaõnnestus
        std::cout << "Akna loomine ebaõnnestus" << std::endl; // Prindime veateate
        glfwTerminate(); // Lõpetame GLFW
        return -1; // Väljuge veaga
    }
    glfwMakeContextCurrent(window); // Teeme akna konteksti aktiivseks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Seame framebufferi suuruse callback'i

    // Laadime kõik OpenGL funktsioonide näitajad GLADi abil
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLADi algatamine ebaõnnestus" << std::endl; // Prindime veateate
        return -1; // Väljuge veaga
    }

    unsigned int vertexShader; // Vertex shaderi ID
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // Loome vertex shaderi
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Määrame shaderi allika
    glCompileShader(vertexShader); // Kompileerime shaderi

    int success = 0; // Muutuja kompileerimise edukuse kontrollimiseks
    char infoLog[512]; // Vea logi sal glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Kontrollime kompileerimise staatust

    if (!success) { // Kui kompileerimine ebaõnnestus
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Saame veateate
        std::cout << "VIGA::SHADER:VERTEX:KOMPILATSIOON_EBAÕNNESTUS\n" << infoLog << std::endl; // Prindime veateate
    }

    unsigned int fragmentShader; // Fragment shaderi ID
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Loome fragment shaderi
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Määrame shaderi allika
    glCompileShader(fragmentShader); // Kompileerime shaderi

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // Kontrollime kompileerimise staatust
    if (!success) { // Kui kompileerimine ebaõnnestus
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // Saame veateate
        std::cout << "VIGA::SHADER::FRAGMENT::KOMPILATSIOON_EBAÕNNESTUS\n" << infoLog << std::endl; // Prindime veateate
    }

    unsigned int shaderProgram; // Shaderi programmi ID
    shaderProgram = glCreateProgram(); // Loome shaderi programmi
    glAttachShader(shaderProgram, vertexShader); // Lisame vertex shaderi programmi
    glAttachShader(shaderProgram, fragmentShader); // Lisame fragment shaderi programmi
    glLinkProgram(shaderProgram); // Linkime shaderi programmi

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // Kontrollime linkimise staatust
    if (!success) { // Kui linkimine ebaõnnestus
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); // Saame veateate
        std::cout << "VIGA:SHADER::PROGRAM::LINKIMINE_EBAÕNNESTUS\n" << infoLog << std::endl; // Prindime veateate
    }

    glDeleteShader(vertexShader); // Kustutame vertex shaderi
    glDeleteShader(fragmentShader); // Kustutame fragment shaderi

    float vertices[] = { // Vertexide andmed
        // positsioonid         // värvid           // tekstuurikoordinaadid
        0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // ülemine parem
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // alumine parem
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // alumine vasak
        -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // ülemine vasak
    };

    unsigned int indices[] = { // Indeksid
        0, 1, 3, // esimene kolmnurk
        1, 2, 3  // teine kolmnurk
    };

    unsigned int VBO, VAO, EBO; // Vertex Buffer Object, Vertex Array Object, Element Buffer Object
    glGenBuffers(1, &VBO); // Loome VBO
    glGenVertexArrays(1, &VAO); // Loome VAO
    glGenBuffers(1, &EBO); // Loome EBO

    glBindVertexArray(VAO); // Seome VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Seome VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Laadime vertexide andmed

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Seome EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // La adime indeksite andmed

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Määrame positsiooni atribuudid
    glEnableVertexAttribArray(0); // Aktiveerime positsiooni atribuudid

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Määrame värvi atribuudid
    glEnableVertexAttribArray(1); // Aktiveerime värvi atribuudid

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Määrame tekstuurikoordinaadi atribuudid
    glEnableVertexAttribArray(2); // Aktiveerime tekstuurikoordinaadi atribuudid

    // Laadime tekstuuri
    unsigned int texture; // Tekstuuride ID
    glGenTextures(1, &texture); // Loome tekstuuri
    glBindTexture(GL_TEXTURE_2D, texture); // Seome tekstuuri

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Määrame tekstuuri wrap S
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Määrame tekstuuri wrap T
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Määrame minifiltri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Määrame magfiltri

    int width, height, nrChannels; // Muutujad pildi laiuse, kõrguse ja kanalite arvu jaoks
    unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0); // Laadime tekstuuri pildi

    if (data) { // Kui pildi laadimine õnnestus
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // Määrame tekstuuri andmed
        glGenerateMipmap(GL_TEXTURE_2D); // Loome mipmapid
    }
    else { // Kui pildi laadimine ebaõnnestus
        std::cout << "Tekstuuride laadimine ebaõnnestus" << std::endl; // Prindime veateate
    }
    stbi_image_free(data); // Vabastame pildi andmed

    while (!glfwWindowShouldClose(window)) { // Peamine tsükkel, kuni aken on avatud
        processInput(window); // Töötleme sisendit

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Määrame taustavärvi
        glClear(GL_COLOR_BUFFER_BIT); // Selgitame ekraani

        glUseProgram(shaderProgram); // Kasutame shaderi programmi
        glBindTexture(GL_TEXTURE_2D, texture); // Seome tekstuuri
        glBindVertexArray(VAO); // Seome VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Joonistame kolmnurgad

        glfwSwapBuffers(window); // Vahetame puhverid
        glfwPollEvents(); // Käsitleme sündmusi
    }

    glDeleteVertexArrays(1, &VAO); // Kustutame VAO
    glDeleteBuffers(1, &VBO); // Kustutame VBO
    glDeleteBuffers(1, &EBO); // Kustutame EBO
    glDeleteProgram(shaderProgram); // Kustutame shaderi programmi
    glDeleteTextures(1, &texture); // Kustutame tekstuuri

    glfwTerminate(); // Lõpetame GLFW
    return 0; // Väljuge edukalt
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { // Callback akna suuruse muutmiseks
    glViewport(0, 0, width, height); // Määrame vaateporti
}

void processInput(GLFWwindow* window) { // Funktsioon sisendi töötlemiseks
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Kui ESC klahvi vajutatakse
        glfwSetWindowShouldClose(window, true); // Sulgeme akna
}