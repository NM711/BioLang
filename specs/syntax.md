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
    private myField: int = 0;
    public myMethod (): void {
      this->myField = 10;
    };
  };
```
