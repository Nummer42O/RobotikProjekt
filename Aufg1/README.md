### Vorbereitung

#### 1.

```c++
#include <iostream> //std::cout & std::endl
#include <stdlib.h> //EXIT_FAILURE & EXIT_SUCCESS

int main(int argc, char* argv[]) { //function header with command line input
    if (argc == 1) { //if no command line arugment
        std::cout << "Hello World!" << std::endl; //default print
    } else if (argc == 2) { //if command line argument
        std::cout << "Hello " << argv[1] << '!' << std::endl; //print "Hello <name>!"
    } else { //if too many command line arugments
        std::cerr << "Invalid argument: " << argv[2] << '\n' //print help
                  << "Usage:\n"
                  << argv[0] << " [name (optional)]" << std::endl;

        return EXIT_FAILURE; //function fails due to invalid command line use
    }

    return EXIT_SUCCESS; //function succeeds
} //end of function scope
```

#### 2.

##### Datentypen

Datentyp      | Speichergröße in byte
--------------|----------------------
`char`        | 1
`int`         | 4
`short int`   | 2
`long int`    | 8
`float`       | 4
`double`      | 8
`long double` | 12

##### Referenz vs. Pointer

 - Pointer: Speichert Speicheradresse eines Objektes
 - Referenz: Ein Alias einer Variable, d.h. eine 2. Variable für das **selbe** Objekt

##### Funktionen-/Klassendefinition

###### Funktionen

```c++
<return-type> <function-name>(<parameter-type1> <optional-parameter-name>, ...) {
    <body>

    return ...;
}
```

###### Klassen

```c++
class <class-name> {
    public:
        ...
    private:
        ...
    protected:
        ...
}
```

##### value types in Expressions

 - `glvalue`: Beschreibt das Objekt/Bestimmt die Identität des Objekts
 - `prvalue`: Initialisiert ein Objekt bzw. errechnet ein neues
 - `xvalue`: erneut nutzbares `glvalue`
 - `lvalue`: `glvalue`, welches kein `xvalue` ist
 - `rvalue`: `prvalue` oder `xvalue`

#### 3.

##### Unterschied

Arrays und Variablen, welche im Stack angelegt werden sollen, brauchen eine statische, zur Compiletime bekannte Größe. Das Speichermanagement dieser Variablen werden dabei com Compiler übernommen.
Falls z.B. die Länge von Arrays allerdings nicht zur Compiletime bekannt ist, so muss der Speicher im Heap dynamisch alloziert und in Eigenverantwortung des Programmierers wieder gelöst werden.

##### Bsp. Stack

```c++
#include <iostream>

#define N 5

int main(int argc, char* char[]) {
    double array[N];

    for (unsigned int i = 0; i < N; i++) {
        array[i] = i + 1.1;
    }

    //code

    std::cout << array[0];
    for (unsigned int i = 1; i < N; i++) {
        std::cout << ", " << array[i];
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
```

##### Bsp. Heap

```c++
#include <iostream>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int N;
    if (argc == 2) {
        N = atoi(argv[1]);
    } else {
        N = rand() % 10;
    }

    double *array = new double[N];
    /* C-style alternative:
    double *array = malloc(N * sizeof(double));
    */

    for (unsigned int i = 0; i < N; i++) {
        array[i] = i + 1.1;
    }

    //code

    std::cout << array[0];
    for (unsigned int i = 1; i < N; i++) {
        std::cout << ", " << array[i];
    }
    std::cout << std::endl;

    delete[] array; //free memory
    /* C-style alternative:
    free(array);
    */

    return EXIT_SUCCESS;
}
```

#### 4.

Die Rule of Five umfasst 5 definitiv automatisch generierten Klassenmethoden:
 - destructor -> free / delete
 - copy constructor -> erstellt neue lvalues und schreibt die Werte des rvalues darauf
 - move constructor -> nutzt die Werte des rvalues als eigene lvalues
 - copy assignment -> siehe copy constructor
 - move assignment -> siehe move constructor

```c++
#include <stdlib.h>
#include <utility>

template<typename TYPE>
class Test {
private:
    TYPE *ptr;
public:
    //constructor
    Test() { ptr = nullptr; }
    Test(void *p): ptr(static_cast<TYPE *>(p)) {}

    //destructor
    ~Test() { delete ptr; }

    //copy
    Test(const Test<TYPE> &other) {
        delete ptr;
        ptr = other.ptr;
    }
    Test<TYPE> &operator=(const Type<TYPE> &other) {
        delete ptr;
        ptr = other.ptr;
        
        return *this;
    }

    //move
    Test(Test<TYPE> &&other) {
        std::swap(ptr, other.ptr);
    }
    Test<TYPE> &operator=(Test<TYPE> &&other) {
        std::swap(ptr, other.ptr);

        return *this;
    }
};

int main(int argc, char* argv[]) {
    //constructor
    int value = 10;
    Test<int> test_default();
    Test<int> test_custom(&value);

    //copy
    Test<int> test_copy(test_default);
    Test<int> test_copy_assignment = test_default;

    //move
    Test<int> test_move(std::move(test_custom));
    Test<int> test_move_assignment = std::move(test_custom);

    //destructor (implizit)
    return EXIT_SUCCESS;
}
 ```