# Maqueen Package for Micro:bit
繁體中文板翻譯:kodorobot宇宙機器人 源自於:DFRobot

## Control the motor
```blocks
// forward
maqueen.motorRun(maqueen.aMotors.M1, maqueen.Dir.CW, 100)
maqueen.motorRun(maqueen.aMotors.M2, maqueen.Dir.CW, 100)
basic.pause(100)

// stop
maqueen.motorStop(maqueen.aMotors.M1)
maqueen.motorStop(maqueen.aMotors.M2)
basic.pause(100)

// back
maqueen.motorRun(maqueen.aMotors.M1, maqueen.Dir.CCW, 100)
maqueen.motorRun(maqueen.aMotors.M2, maqueen.Dir.CCW, 100)
basic.pause(100)

// stop all
maqueen.motorStopAll()
basic.pause(100)
```

## LED
```blocks
// on
maqueen.writeLED(maqueen.LED.LEDLeft, maqueen.LEDswitch.turnOn)
maqueen.writeLED(maqueen.LED.LEDRight, maqueen.LEDswitch.turnOn)
basic.pause(100)

// off
maqueen.writeLED(maqueen.LED.LEDLeft, maqueen.LEDswitch.turnOff)
maqueen.writeLED(maqueen.LED.LEDRight, maqueen.LEDswitch.turnOff)
basic.pause(100)
```

## Sensor
```blocks
// patrol
let left = maqueen.readPatrol(maqueen.Patrol.PatrolLeft)
let right = maqueen.readPatrol(maqueen.Patrol.PatrolRight)

// sonar
let sonar = maqueen.sensor(PingUnit.Centimeters)
```

## Read IR
```blocks
let data = 0
data = maqueen.IR_read()

maqueen.IR_callbackUser(function ({ myparam: message }) {
    basic.showString(message)
})
```

## License
MIT

## Supported targets
for PXT/microbit (The metadata above is needed for package search.)
