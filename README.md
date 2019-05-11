# Config Parser

Simple .cfg parser for key=value, newline delimeted, config files.

## API

You can include these files and use the following functions to interact with the cfg files.

#### Parse a .cfg file

`Config* cfg_parse(char* filename)` --
```
....

  char* filename = "path/to/example.cfg";
  Config* cfg = cfg_parse(filename);

....

```

#### Get value from Config struct
`char* cfg_get(Config* cfg, char* key)` --
```
....

  char* filename = "path/to/example.cfg";
  Config* cfg = cfg_parse(filename);
  char* key = "hello";
  char* value = cfg_get(cfg, key);
  if (value) {
    printf("Key: %s was not empty, here is the value: %s\n", key, value);
  } else {
    printf("Key: %s was not found\n", key);
  }

....

```

#### Set a value in the Config struct
`char* cfg_put(Config* cfg, char* key, char* value)` --
```
....

  char* filename = "path/to/example.cfg";
  Config* cfg = cfg_parse(filename);
  char* key = "hello";
  char* value = cfg_get(cfg, key);
  char* newVal = " World!";
  if (!value) {
    cfg_put(cfg, key, newVal);
  }

....

```

#### Create a blank config struct
`Config* new_config()` --
```
....

  Config* cfg = new_config();
  char* key = "Hello";
  char* value = " World!";
  cfg_put(cfg, key, value);

....

```

#### Cleanup after usage
`void cfg_cleanup(Config* cfg);` --
```
....

    cfg_cleanup(cfg);
    return 0;
  }

```
