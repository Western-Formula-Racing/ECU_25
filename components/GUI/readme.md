# GUI
Transmits data to and from a webpage through the ESP's Access Point.

## Getting Started
**Before trying to run the GUI, the filesystem must be built and uploaded onto the ESP. To do this, click on the
platformio icon on the sidebar in VS Code, under platform, click on build filesystem image and then upload filesystem
image, while the ESP is connected to your computer, the port to which the ESP is connected is selected.**

To use the GUI, it should be initialized at the start of the code.
```
GUI gui = GUI::Get();
```
>[!IMPORTANT]
>The GUI can be accessed at http://192.168.4.1 when connect to the ESP's AP. 

### Potential Issues
- SPIFFS partition not found: If the SPIFFS partition is not present, the GUI cannot work. To fix this, first make sure
  SPIFFS is present in the partition table (look [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/partition-tables.html#creating-custom-tables) for more info).
  If this has not yet solved the issues make sure the configuration, `board_build.partitions = path-to-partition-table` is in your platformio.ini file for each
  project environment.

## Sendables
Sendables are entries in the webpage that only display values.

Ideally, sendables should be registered at the beginning of the program.

>[!WARNING]
>ALL CALLABLES MUST RETURN A VALUE OF THE SAME TYPE AS THEIR RESPECTIVE SENDABLE

### Display Integer
To display integers on the webpage, use the command
`GUI::register_int_sendable(key, callable)`

- Example:
```
// Display random integer
gui->register_int_sendable("Random Int", rand());

// Display constant integer
gui->register_int_sendable("Constant Int", []() {
  return 1;
});
```

### Display Float
Register float entries using the following command,
`GUI::register_float_sendable(key, callable);`

- Example
```
// Display random float
gui->register_int_sendable("Random Int", []() -> float {
  return static_cast<float>(rand() / static_cast<float>(RAND_MAX));
});

// Display constant float
gui->register_int_sendable("Constant Float", []() -> float {
  return 2.4f;
});
```

### Display String
Register string entries using the following command,
`GUI::register_string_sendable(key, callable)`

- Example
```
gui->register_string_sendable("Some String", []() {
  return "Hello World";
});
```

### Display Bool
Register boolean entries using the following command,
`GUI::register_bool_sendable(key, callable)`

- Example
```
// Checks if a random number is even or odd
int random = rand();
gui->register_bool_sendable("IsEven", []() {
  return random % 2 == 0;
});
```

## Recievables
Recievables are entries in the webpage that send values to the ESP.

>[!CAUTION]
> Recievables that are not registered at beginning of the code might not be registered on the webpage

### IntRecievable
Int Recievables provide input boxes to send integer values to the ESP.

Int Recievables are registered in the command, `GUI::register_int_recievable(char* key, Recievable<int>*);`

Example
```
// Create recievable object
IntRecievable int_recievable{0};

// Register recievable
gui->register_int_reciebable("Int Recievable", &int_recievable);

// Obtain recievable value
int value = int_recievable.get_value();
```

### FloatRecievable
Float Recievables provide input boxes to send float values to the ESP.

FloatRecievables are registered in the command, `GUI::register_float_recievable(char* key, Recievable<float>*);`

Example
```
// Create recievable object
FloatRecievable float_recievable{0.0f};

// Register recievable
gui->register_float_reciebable("Float Recievable", &float_recievable);

// Obtain recievable value
float value = float_recievable.get_value();
```

### StringRecievable
String Recievables provide input boxes to send string values to the ESP.

StringRecievables are registered in the command, `GUI::register_string_recievable(char* key, Recievable<char*>*);`

Example
```
// Create recievable object
StringRecievable string_recievable{""};

// Register recievable
gui->register_string_reciebable("String Recievable", &string_recievable);

// Obtain recievable value
char* str = string_recievable.get_value();
```

### Option
Options provide select inputs on the webpage to send some option input to the ESP.

Option Recievables are registered based on their type, so if and option recievable
is of type _char*_ the command, `GUI::register_string_recievable(char* key, Recievable<char*>*);`
would be appropriate.

>[!WARNING]
>Option Recievables can only have the types int, float, and string

Example
```
// Create string option object
Option<char*> idle_mode_option{{"Brake Mode", "Coast Mode"}, 1}; // Initial mode is Coast mode at index 1

// Create float option object
Option<float> float_option{{2.3, 6.7, 3.2}}; // Initial Value at index 0

// Register options
gui->register_string_recievable("Idle Mode", &idle_mode_option);
gui->register_float_recievable("Float Option", &float_option);

// Access values
char* current_idle_mode = idle_mode_option.get_value();
float value = float_option.get_value()
```

### ButtonRecievable
Button Recievables provide a button on the webpage that execute some callable on the ESP.

Button Recievables are registered using the command `GUI::register_button_recievable(char* key, ButtonRecievable*);`

>[!CAUTION]
> Callables that return some value may result in the code crashing.

Example
```
// Hello world function
void helloWorld() {
  printf("Hello World");
}

// Create Button Recievable object
ButtonRecievable{helloWorld};

// Register button recievable
gui->register_button_recievable("Hello World", helloWorld);
```
