# MotoresGraficos R1-R2

Proyecto desarrollado en Unreal Engine 5.8 utilizando C++ y Blueprints.

El objetivo de la práctica es crear un personaje jugable básico, combinando lógica implementada en C++ con configuración visual y de animaciones mediante Blueprints.

## GameMode y personaje jugable

Se ha creado un `GameMode` propio en C++ mediante la clase:

- `AMyGameMode`

A partir de esta clase se ha creado el Blueprint:

- `BP_MyGameMode`

Este GameMode utiliza como personaje principal:

- `BP_PlayerCharacter`

El personaje está basado en la clase C++:

- `APlayerCharacter`

También se ha creado un mapa básico utilizado como escenario principal del proyecto.

## Movimiento y cámara

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

## Animaciones Idle y Run

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

## Salto y disparo

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

## Sistema de disparo mediante Line Trace

Primero se realiza un trace desde la cámara para determinar el punto al que está apuntando el jugador. 
Posteriormente se realiza el disparo real desde el socket `MuzzleSocket` situado en el cañón del rifle hacia el punto determinado anteriormente.

El personaje también rota hacia la dirección de apuntado al disparar.

Se han añadido las siguientes propiedades configurables:

- WeaponDamage: daño realizado por cada impacto.
- FireRate: intervalo entre disparos.

El Fire Rate utiliza un Timer para impedir que mantener pulsado el botón de disparo aplique daño una vez por frame.

También se ha añadido una cruceta mediante: WBP_Crosshair que se muestra permanentemente en el centro de la pantalla.

## Plataformas móviles

Se ha creado la clase C++ `AMovingPlatform` y su correspondiente Blueprint reutilizable.

Las plataformas disponen de dos puntos configurables:

- PointA
- PointB

Cada plataforma se desplaza continuamente entre ambos puntos.

El movimiento incorpora:

- Aceleración progresiva al comenzar.
- Velocidad máxima configurable.
- Frenado progresivo al aproximarse al destino.

Pausa configurable en ambos extremos.

En el escenario se utilizan:

- 2 plataformas con movimiento vertical.
- 1 plataforma con movimiento horizontal.

## Puertas interactuables

Se ha creado la clase `ADoorActor`.

El jugador puede interactuar con una puerta cuando se encuentra dentro de su zona de interacción.

Se ha creado la acción `IA_Interact` asociada a la tecla `E`.

La puerta detecta al jugador mediante un BoxComponent.

Cuando el jugador está suficientemente cerca, la puerta se registra como la puerta actualmente interactuable.

Al pulsar E:

- Si está cerrada, se abre;
- Si está abierta, se cierra.

La rotación de la puerta se interpola progresivamente para conseguir un movimiento suave.

## Enemigos y sistema de daño

Se ha creado la clase `AEnemyActor`.

Los enemigos son actores estáticos representados mediante un Static Mesh.

Cada enemigo dispone de una cantidad configurable de vida `Health`. Al llegar a cero mediante los impactos del player, se destruye.

## Portal

Se ha creado la clase `APortalActor`.

Los portales funcionan como parejas enlazadas, Cada instancia dispone de una referencia que indica cuál es su portal de destino.
Permiten realizar un teletransporte bidireccional.

El jugador es detectado mediante un BoxComponent.

Al atravesar el portal:

Se teletransporta hasta el portal enlazado.

Aparece delante del portal de salida.

El personaje rota siguiendo la dirección de salida.

Se aplica un pequeño cooldown para impedir que el jugador sea teletransportado inmediatamente de vuelta al portal anterior.

Visualmente, cada portal está construido utilizando dos Torus (creados con blender) y una con un material animado (una imagen también creada con IA).

El material utiliza desplazamiento de coordenadas mediante un Panner, generando un efecto de energía en movimiento.

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
