# MotoresGraficos_R1

Proyecto desarrollado en Unreal Engine 5.8 utilizando C++ y Blueprints.

El objetivo de la práctica es crear un personaje jugable básico, combinando lógica implementada en C++ con configuración visual y de animaciones mediante Blueprints.

## Demo

[![Watch the gameplay demo](https://img.youtube.com/vi/ftC4xGEnZGc/maxresdefault.jpg)](https://www.youtube.com/watch?v=ftC4xGEnZGc)

## Ejercicio 1 - GameMode y personaje jugable

Se ha creado un `GameMode` propio en C++ mediante la clase:

- `AMyGameMode`

A partir de esta clase se ha creado el Blueprint:

- `BP_MyGameMode`

Este GameMode utiliza como personaje principal:

- `BP_PlayerCharacter`

El personaje está basado en la clase C++:

- `APlayerCharacter`

También se ha creado un mapa básico utilizado como escenario principal del proyecto.

## Ejercicio 2 - Movimiento y cámara

El movimiento del personaje está implementado en C++ utilizando el sistema `Enhanced Input`.

Se han creado las siguientes acciones de entrada:

- `IA_Move`
- `IA_Look`

Y un contexto de entrada:

- `IMC_Player`

El personaje puede desplazarse utilizando las teclas WASD.

El movimiento se calcula en función de la orientación de la cámara, permitiendo que el personaje avance en la dirección hacia la que está mirando el jugador.

La cámara se controla mediante el movimiento del ratón y utiliza:

- `AddControllerYawInput`
- `AddControllerPitchInput`

La lógica de movimiento y cámara se encuentra en `PlayerCharacter.cpp`.

## Ejercicio 3 - Animaciones Idle y Run

Se ha creado un Animation Blueprint:

- `ABP_PlayerCharacter`

El personaje calcula en C++ su velocidad horizontal mediante:

`GetVelocity().Size2D()`

Este valor se expone al Animation Blueprint mediante la variable:

- `Speed`

La máquina de estados del Animation Blueprint utiliza esta variable para cambiar entre:

- Idle
- Run

De esta forma, el personaje reproduce la animación Idle cuando está parado y la animación Run cuando se encuentra en movimiento.

## Ejercicio 4 - Salto y disparo

### Salto

Se ha creado la acción:

- `IA_Jump`

El salto se gestiona desde C++ utilizando las funciones propias de `ACharacter`:

- `Jump()`
- `StopJumping()`

El Animation Blueprint utiliza el estado `Is Falling` del componente `CharacterMovement` para detectar cuándo el personaje se encuentra en el aire y reproducir la animación de salto.

### Disparo

Se ha creado la acción:

- `IA_Shoot`

El disparo se gestiona desde C++ mediante:

- `StartShooting()`
- `StopShooting()`

La animación de disparo utiliza un `Animation Montage`.

Se ha creado un Slot específico:

- `UpperBody`

y se utiliza un nodo `Layered Blend Per Bone` para permitir que la animación de disparo afecte principalmente a la parte superior del cuerpo mientras las piernas continúan reproduciendo las animaciones de locomoción.

De esta forma, el personaje puede correr y disparar simultáneamente.

Mantener pulsado el botón izquierdo del ratón mantiene la animación de disparo activa, y al soltarlo el Montage se detiene.

## Arma

Se ha añadido un rifle como `Static Mesh` al personaje.

El arma está acoplada mediante un socket al esqueleto del personaje, permitiendo que siga correctamente el movimiento de la mano.

También se ha creado y configurado un material para el rifle utilizando las texturas disponibles:

- Base Color
- Normal
- Occlusion / Roughness

El mesh del arma proviene gratuitamente del FAB de Epic:
(`https://www.fab.com/listings/8e200050-3158-4762-b297-f785b5b1533d`)

Mientras que el Mesh del personaje puede encontrarse en Mixamo:
(`https://www.mixamo.com/`)

## Estructura general

La lógica principal del personaje se encuentra implementada en C++:

- Movimiento
- Cámara
- Salto
- Disparo
- Cálculo de velocidad

Los Blueprints se utilizan principalmente para:

- Configurar assets
- Asignar Inputs
- Configurar el personaje
- Gestionar la máquina de estados de animación
- Configurar el Animation Montage y el blend de animaciones

## Controles

- W / A / S / D: Movimiento
- Ratón: Cámara
- Espacio: Saltar
- Botón izquierdo del ratón: Disparar
