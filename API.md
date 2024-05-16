# API Documentation

## `BehaviourTree`
The primary node used to configure the Behaviour Tree and to store all the nodes inside of it.

```xml
<!-- cycle_limit of 0 is unlimited -->
<BehaviourTree cycle_limit="0">
    <!-- Child nodes here -->
</BehaviourTree>
```

### `Root`
The 'functions' of the Behaviour Tree. Any Root with id "Main" will always be the starting point; otherwise, the topmost Root node is used.

If you want more than one child node in the Root, please look at Sequence / Selector nodes.

```xml
<BehaviourTree>
    <Root id="Main">
        <!-- Child nodes here -->
    </Root>
    <Root id="Other">
        <!-- Child nodes here -->
    </Root>
</BehaviourTree>
```

### `UseRoot`
Calls the specified Root node by id.

```xml
<BehaviourTree>
    <Root id="Main">
        <UseRoot id="Other"/>
    </Root>
    <Root id="Other">
        <!-- Child nodes here -->
    </Root>
</BehaviourTree>
```

### `Selector`
Composite node.  
Goes through each child node sequentially and returns if any node returns SUCCESS.

Returns SUCCESS if it is returned early; otherwise, it will return FAILURE.

```xml
<BehaviourTree>
    <Root id="Main">
        <Selector>
            <Action:Print text="1"/>
            <Action:Print text="2"/>
            <Action:Print text="3"/>
        </Selector>
    </Root>
</BehaviourTree>
<!-- 
    Output:
    1
 -->
```

### `Sequence`
Composite node.  
Goes through each child node sequentially and returns if any node returns FAILURE.

Returns SUCCESS if it executes all of the child nodes; otherwise, it will return FAILURE when exited early.

```xml
<BehaviourTree>
    <Root id="Main">
        <Sequence>
            <Action:Print text="1"/>
            <Action:Print text="2"/>
            <Action:Print text="3"/>
        </Sequence>
    </Root>
</BehaviourTree>
<!-- 
    Output:
    1
    2
    3
 -->
```

### `Random`
Composite node.  
Randomly chooses one of its child nodes.

```xml
<BehaviourTree>
    <Root id="Main">
        <Random>
            <Action:Print text="1"/>
            <Action:Print text="2"/>
            <Action:Print text="3"/>
        </Random>
    </Root>
</BehaviourTree>
<!-- 
    Output:
    2
 -->
```

### `Invert`
Decorator node.  
Inverts the status returned by its child node.

- SUCCESS => FAILURE  
- FAILURE => SUCCESS  
- RUNNING => RUNNING

```xml
<BehaviourTree>
    <Root id="Main">
        <Invert>
            <Succeed/>
        </Invert>
    </Root>
</BehaviourTree>
<!-- 
    Returns:
    FAILURE
 -->
```

### `Repeat`
Decorator node.  
Repeats running the child node 'count' times and can 'break_on_fail'.

- `count`: integer
- `break_on_fail`: "true" | "false"

```xml
<BehaviourTree>
    <Root id="Main">
        <Repeat count="3" break_on_fail="true">
            <Action:Print text="Hello World"/>
        </Repeat>
    </Root>
</BehaviourTree>
<!-- 
    Outputs:
    Hello World
    Hello World
    Hello World
 -->
```

### `Fail`
Returns FAILURE.

```xml
<BehaviourTree>
    <Root id="Main">
        <Fail/>
    </Root>
</BehaviourTree>
<!-- 
    Returns:
    FAILURE
 -->
```

### `Succeed`
Returns SUCCESS.

```xml
<BehaviourTree>
    <Root id="Main">
        <Succeed/>
    </Root>
</BehaviourTree>
<!-- 
    Returns:
    SUCCESS
 -->
```

### `Blackboard:ChangeInteger`
Able to use the Blackboard to store and change variables.

- `variable_name`: string
- `value`: integer
- `integer_change_type`: "Set" | "Add" | "Subtract"

```xml
<BehaviourTree>
    <Root id="Main">
        <Sequence>
            <Blackboard:ChangeInteger variable_name="test" value="42" integer_change_type="Set"/>
            <Blackboard:ChangeInteger variable_name="test" value="1" integer_change_type="Add"/>
            <Blackboard:ChangeInteger variable_name="test" value="40" integer_change_type="Subtract"/>
        </Sequence>
    </Root>
</BehaviourTree>
<!-- 
    Initializes variable called 'test' with value 42.
    Adds 1 to the variable 'test' resulting in value 43.
    Subtracts 40 from the variable 'test' resulting in value 3.
 -->
```

### `Blackboard:IntegerCondition`
Returns the relevant status based on its specified 'condition_operator_type'.

- `variable_name`: string
- `value`: integer
- `condition_operator_type`: "=" | "!=" | "<" | ">" | ">="

```xml
<BehaviourTree>
    <Root id="Main">
        <Sequence>
            <Blackboard:ChangeInteger variable_name="test" value="42" integer_change_type="Set"/>
            <Blackboard:IntegerCondition variable_name="test" value="42" condition_operator_type="="/>
        </Sequence>
    </Root>
</BehaviourTree>
<!-- 
    Initializes variable called 'test' with value 42.
    Returns SUCCESS as the 'test' variable is 42.
 -->
```

### `Action:PauseExecution`
Returns status RUNNING until the specified 'ms' has passed.

- `ms`: integer

```xml
<BehaviourTree>
    <Root id="Main">
        <Sequence>
            <Action:PauseExecution ms="1000"/>
            <Action:Print text="Success"/>
        </Sequence>
    </Root>
</BehaviourTree>
<!-- 
    Waits 1 second
    Outputs:
    Success
 -->
```

### `Action:Print`
Outputs 'text' to the console.

- `text`: string

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:Print text="Hello World"/>
    </Root>
</BehaviourTree>
<!-- 
    Output:
    Hello World
 -->
```

### `Action:Drive`
Controls driving actions.

- `speed`: integer (range 0-100)
- `direction_type`: "Forward" | "Backward"

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:Drive speed="50" direction_type="Forward"/>
    </Root>
</BehaviourTree>
```

### `Action:Turn`
Controls turning actions.

- `angle`: integer (range 0-180)

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:Turn angle="90"/>
    </Root>
</BehaviourTree>
```

### `Action:SetAngle`
Sets the angle for a servo.

- `servo_type`: "FrontWheels" | "CameraServo1" | "CameraServo2"
- `angle`: integer (range 0-180)

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:SetAngle servo_type="FrontWheels" angle="90"/>
    </Root>
</BehaviourTree>
```

### `Action:SetSpeed`
Sets the speed for a wheel.

- `wheel_type`: "Left" | "Right" | "Both"
- `speed`: integer (range 0-100)

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:SetSpeed wheel_type="Both" speed="75"/>
    </Root>
</BehaviourTree>
```

### `Action:SetWheelDirection`
Sets the direction for a wheel.

- `wheel_type`: "Left" | "Right" | "Both"
- `direction_type`: "Forward" | "Backward"

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:SetWheelDirection wheel_type="Both" direction_type="Backward"/>
    </Root>
</BehaviourTree>
```

### `Condition:SucceedOnAverageNearbyScan`
Succeeds based on the average of nearby scans.

- `min_angle`: integer (range 0-360)
- `max_angle`: integer (range 0-360)
- `cm`: integer
- `smallest_measure_amount_used`: integer

```xml
<BehaviourTree>
    <Root id="Main">
        <Condition:SucceedOnAverageNearbyScan min_angle="0" max_angle="180" cm="10" smallest_measure_amount_used="5"/>
    </Root>
</BehaviourTree>
```

### `Condition:SucceedOnAnyNearbyScan`
Succeeds if any nearby scan meets the criteria.

- `min_angle`: integer (range 0-360)
- `max_angle`: integer (range 0-360)
- `cm`: integer

```xml
<BehaviourTree>
    <Root id="Main">
        <Condition:SucceedOnAnyNearbyScan min_angle="0" max_angle="180" cm="10"/>
    </Root>
</BehaviourTree>
```

### `Condition:SucceedOnAverageColour`
Succeeds based on the average colour detected.

- `hex_colour`: string (hexadecimal colour code)
- `tolerance`: integer (range 0-100)

```xml
<BehaviourTree>
    <Root id="Main">
        <Condition:SucceedOnAverageColour hex_colour="#FFFFFF" tolerance="10"/>
    </Root>
</BehaviourTree>
```