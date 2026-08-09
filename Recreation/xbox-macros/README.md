# Xbox Controller Macros
The purpose of this Go package is to provide a command line interface to perform macros in games using a standard Xbox controller or generic ones that use the same input.

Supported inputs include A, B, X, Y, LB, RB, LT, RT, UP, DOWN, LEFT, RIGHT, START, BACK, LS, and RS.

## Usage
Run the following command in the CLI.
```bash
go run .
```
Type in the trigger for the given profile json like the example below
```bash
> LT LT 
```

Then you will have 10 seconds to input the macro sequence like
```bash
> LT wait:150 LT
```
If properly matched will spit out the desired controller output. For now this is shown as a json body.
```json
{
  "raw": "A wait:100 B",
  "steps": [
    {
      "inputs": [
        "A"
      ]
    },
    {
      "delay_ms": 100
    },
    {
      "inputs": [
        "B"
      ]
    }
  ]
}
```