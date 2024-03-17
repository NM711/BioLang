## Comments

You can write comments in your code with "#", example below:

```python

# This is a comment above the hello string!

const hello: string = "hello";

```

Comments are terminated after a new line.


## Variable Declaration

Variables are declared and initialized as follows:

```python
  # Variable

  let myString: string = "hello world";

  let myInt: int;

  myInt = 5;

  # Constant

  const message: string = "this is my message!";

  # This will throw an error, constants MUST be initialized
  const myConst: int;
```

## Function Declaration

Function declaration syntax below:

```rust
fn myFn(): void {
  let i: int = 0;
  i++;
};
```

## Object Declaration

Objects are very similar to structs but, with their own twist. Unlike structs in an object you can store
fields and methods, you can set them to private and public.

Also you may have noticed that unlike variables, objects are declared in their own specific way. Objects are special
data structures and so I just thought it would be right to declare them in a different manner, similar to structs.

Objects is one way in which you can achieve composition in BioLang, objects can contain object fields and so on.

```ts
  object MyObject {
    private const myField: int = 0;
    private let index: int = 1;

    public fn myMethod (): void {
      this->myField = 10;
    };
  };
```

## Modules

(Module System has not been implemented yet)


For the module system there is a few ways in which you can export declaration the first is by
having an "export" keyword next to the declaration you wish to export. Note that exporting from a module
will only work if you have a valid "module" keyword at the top level of the file and a name for the module you are 
working on. An example can be seen below.

```rust
  module "math";

  export fn add: int (x: int, y: int) {
    return x + y;
  };

  export fn subtrat: int (x: int, y: int) {
    return x - y;
  };

```

Also note that when you use the export keyword, you are essentially creating a contract between you and the module. You are
saying that those declarations you are exporting will be exposed to anyone, they are automatically by default always going
to be public to be used anywhere until, you remove the export.

Now for importing the idea is pretty straight forward, all you must do is set the path to the module and import it by the
name you assigned to it.

Modules essentially work as one big object, you can have more objects, functions, variables, etc, within it.

```python
  import math from "/your/file/path.bl"

  fn main(): int {
    math->add(10, 3)

    return 0;
  };
```
