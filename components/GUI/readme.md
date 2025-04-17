# GUI
Transmits data to and from a webpage through the ESP's Access Point. 

To use the GUI, it should be initialized at the start of the code like the following,
```
GUI gui = GUI::Get();
```

## Sendables
Sendables are entries sent to the webpage to display some data.

Ideally, sendables should be registered at the beginning of the program.

**ALL CALLABLES MUST RETURN A VALUE OF THE SAME TYPE AS THEIR RESPECTIVE SENDABLE**

### Display Integer
To display integers on the webpage, use the command
`GUI::register_int_sendable(key, callable)`

- Example:
```
// Display random integer
GUI::register_int_sendable("Random Int", []() {
  return rand();
});

// Display constant integer
GUI::register_int_sendable("Constant Int", []() {
  return 1;
});
```

### Display Float
Register float entries using the following command,
`GUI::register_float_sendable(key, callable);`

- Example
```
// Display random float
GUI::register_int_sendable("Random Int", []() {
  return (float)rand()/4;
});

// Display constant float
GUI::register_int_sendable("Constant Int", []() {
  return 2.4f;
});
```

### Display String
Register string entries using the following command,
`GUI::register_string_sendable(key, callable)`

- Example
```
GUI::register_string_sendable("Some String", []() {
  return "String";
});
```

### Display Bool
Register boolean entries using the following command,
`GUI::register_bool_sendable(key, callable)`

- Example
```
// Checks if a random number is even or odd
int random = rand();
GUI::register_bool_sendable("IsEven", []() {
  return random % 2 == 0;
});
```

## Recievables
Work in progress...
