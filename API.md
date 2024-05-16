# API Documentation

## Table of Contents

1. [Overview](#overview)
2. [`BehaviourTree`](#behaviourtree)
3. [Nodes](#nodes)
   - [`Root`](#root)
   - [`UseRoot`](#useroot)
4. [Composite Nodes](#composite-nodes)
   - [`Selector`](#selector)
   - [`Sequence`](#sequence)
   - [`Random`](#random)
5. [Decorator Nodes](#decorator-nodes)
   - [`Invert`](#invert)
   - [`Repeat`](#repeat)
6. [Action Nodes](#action-nodes)
   - [`Fail`](#fail)
   - [`Succeed`](#succeed)
   - [`Action:PauseExecution`](#actionpauseexecution)
   - [`Action:Print`](#actionprint)
   - [`Action:Drive`](#actiondrive)
   - [`Action:Turn`](#actionturn)
   - [`Action:SetAngle`](#actionsetangle)
   - [`Action:SetSpeed`](#actionsetspeed)
   - [`Action:SetWheelDirection`](#actionsetwheeldirection)
7. [Condition Nodes](#condition-nodes)
   - [`Blackboard:ChangeInteger`](#blackboardchangeinteger)
   - [`Blackboard:IntegerCondition`](#blackboardintegercondition)
   - [`Condition:SucceedOnAverageNearbyScan`](#conditionsucceedonaveragenearbyscan)
   - [`Condition:SucceedOnAnyNearbyScan`](#conditionsucceedonanynearbyscan)
   - [`Condition:SucceedOnAverageColour`](#conditionsucceedonaveragecolour)

## Overview
This document provides detailed information about the Behavior Tree nodes and their configuration for creating complex AI behaviors.

## `BehaviourTree`
The root node used to configure the Behavior Tree and store all its child nodes.

### Attributes
- `cycle_limit`: (integer) The number of cycles the tree can run. `0` indicates unlimited cycles.

```xml
<BehaviourTree cycle_limit="0">
    <!-- Child nodes here -->
</BehaviourTree>
```

## Nodes

### `Root`
Defines the starting point(s) of the Behavior Tree. The `Root` node with `id="Main"` is the primary entry point. If not specified, the topmost `Root` node is used.

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
Calls another `Root` node by its `id`.

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

## Composite Nodes

### `Selector`
Executes each child node sequentially until one returns `SUCCESS`. If any node returns `SUCCESS`, it returns `SUCCESS`; otherwise, it returns `FAILURE`.

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
<!-- Output: 1 -->
```

### `Sequence`
Executes each child node sequentially until one returns `FAILURE`. If all nodes return `SUCCESS`, it returns `SUCCESS`; otherwise, it returns `FAILURE`.

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
<!-- Output: 1, 2, 3 -->
```

### `Random`
Randomly selects and executes one of its child nodes.

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
<!-- Output: Randomly 1, 2, or 3 -->
```

## Decorator Nodes

### `Invert`
Inverts the result of its child node.

- `SUCCESS` becomes `FAILURE`
- `FAILURE` becomes `SUCCESS`
- `RUNNING` remains `RUNNING`

```xml
<BehaviourTree>
    <Root id="Main">
        <Invert>
            <Succeed/>
        </Invert>
    </Root>
</BehaviourTree>
<!-- Returns: FAILURE -->
```

### `Repeat`
Repeats execution of its child node a specified number of times, with an option to stop on failure.

- `count`: (integer) Number of repetitions.
- `break_on_fail`: (boolean) Whether to stop on failure.

```xml
<BehaviourTree>
    <Root id="Main">
        <Repeat count="3" break_on_fail="true">
            <Action:Print text="Hello World"/>
        </Repeat>
    </Root>
</BehaviourTree>
<!-- Output: Hello World, Hello World, Hello World -->
```

## Action Nodes

### `Fail`
Immediately returns `FAILURE`.

```xml
<BehaviourTree>
    <Root id="Main">
        <Fail/>
    </Root>
</BehaviourTree>
<!-- Returns: FAILURE -->
```

### `Succeed`
Immediately returns `SUCCESS`.

```xml
<BehaviourTree>
    <Root id="Main">
        <Succeed/>
    </Root>
</BehaviourTree>
<!-- Returns: SUCCESS -->
```

### `Action:PauseExecution`
Pauses execution for a specified number of milliseconds.

- `ms`: (integer) Duration in milliseconds.

```xml
<BehaviourTree>
    <Root id="Main">
        <Sequence>
            <Action:PauseExecution ms="1000"/>
            <Action:Print text="Success"/>
        </Sequence>
    </Root>
</BehaviourTree>
<!-- Output after 1 second: Success -->
```

### `Action:Print`
Prints specified text to the console.

- `text`: (string) The text to print.

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:Print text="Hello World"/>
    </Root>
</BehaviourTree>
<!-- Output: Hello World -->
```

### `Action:Drive`
Controls driving actions with speed and direction.

- `speed`: (integer) Speed value (0-100).
- `direction_type`: (string) "Forward" or "Backward".

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:Drive speed="50" direction_type="Forward"/>
    </Root>
</BehaviourTree>
```

### `Action:Turn`
Controls turning actions with a specified angle.

- `angle`: (integer) Angle value (0-180).

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:Turn angle="90"/>
    </Root>
</BehaviourTree>
```

### `Action:SetAngle`
Sets the angle for a specified servo.

- `servo_type`: (string) "FrontWheels", "CameraServo1", "CameraServo2".
- `angle`: (integer) Angle value (0-180).

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:SetAngle servo_type="FrontWheels" angle="90"/>
    </Root>
</BehaviourTree>
```

### `Action:SetSpeed`
Sets the speed for specified wheels.

- `wheel_type`: (string) "Left", "Right", "Both".
- `speed`: (integer) Speed value (0-100).

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:SetSpeed wheel_type="Both" speed="75"/>
    </Root>
</BehaviourTree>
```

### `Action:SetWheelDirection`
Sets the direction for specified wheels.

- `wheel_type`: (string) "Left", "Right", "Both".
- `direction_type`: (string) "Forward" or "Backward".

```xml
<BehaviourTree>
    <Root id="Main">
        <Action:SetWheelDirection wheel_type="Both" direction_type="Backward"/>
    </Root>
</BehaviourTree>
```

## Condition Nodes

### `Blackboard:ChangeInteger`
Modifies an integer variable in the blackboard.

- `variable_name`: (string) Name of the variable.
- `value`: (integer) Value to set/add/subtract.
- `integer_change_type`: (string) "Set", "Add", "Subtract".

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
<!-- 'test' = 42 -> 43 -> 3 -->
```

### `Blackboard:IntegerCondition`
Evaluates a condition on an integer variable in the blackboard.

- `variable_name`: (string) Name of the variable.
- `value`: (integer) Value to compare against.
- `condition_operator_type`: (string) "=", "!=", "<", ">", ">=".

```xml
<BehaviourTree>
    <Root id="Main">
        <Sequence>
            <Blackboard:ChangeInteger variable_name="test" value="42" integer_change_type="Set"/>
            <Blackboard:IntegerCondition variable_name="test" value="42" condition_operator_type="="/>
       

 </Sequence>
    </Root>
</BehaviourTree>
<!-- Returns: SUCCESS (test == 42) -->
```

### `Condition:SucceedOnAverageNearbyScan`
Succeeds based on the average of nearby scans.

- `min_angle`: (integer) Minimum angle (0-360).
- `max_angle`: (integer) Maximum angle (0-360).
- `cm`: (integer) Distance in centimeters.
- `smallest_measure_amount_used`: (integer) Number of measurements to use.

```xml
<BehaviourTree>
    <Root id="Main">
        <Condition:SucceedOnAverageNearbyScan min_angle="0" max_angle="180" cm="10" smallest_measure_amount_used="5"/>
    </Root>
</BehaviourTree>
```

### `Condition:SucceedOnAnyNearbyScan`
Succeeds if any nearby scan meets the criteria.

- `min_angle`: (integer) Minimum angle (0-360).
- `max_angle`: (integer) Maximum angle (0-360).
- `cm`: (integer) Distance in centimeters.

```xml
<BehaviourTree>
    <Root id="Main">
        <Condition:SucceedOnAnyNearbyScan min_angle="0" max_angle="180" cm="10"/>
    </Root>
</BehaviourTree>
```

### `Condition:SucceedOnAverageColour`
Succeeds based on the average color detected.

- `hex_colour`: (string) Hexadecimal color code.
- `tolerance`: (integer) Tolerance level (0-100).

```xml
<BehaviourTree>
    <Root id="Main">
        <Condition:SucceedOnAverageColour hex_colour="#FFFFFF" tolerance="10"/>
    </Root>
</BehaviourTree>
```
