# Trabajo Práctico Sistemas Operativos - Grupo 19

## Compilación y ejecución

Todo el código fue compilado usando la imagen de docker provista por la cátedra (agodio/itba-so-multi-platform:3.0). Los testeos afuera de qemu también fueron realizados en el contenedor.

El resto del proyecto fue ejecutado usando qemu.

### Generación y ejecución imagen

Para compilar el proyecto usando el bitmap memory manager ejecutar (en el root del proyecto):
```bash
make
```

para compilar usando el buddy memory manager ejecutar:
```bash
make buddy
```

luego para ejecutar el proyecto usando qemu, ejecutar:
```bash
./run.sh
```
NOTA: en run.sh hay dos opciones para ejecutar el proyecto, dependiendo en la versión que usen de qemu las opciones de sonido son diferentes.

### Test memory manager afuera de la imagen del SO

Para generar los tests ejecutables afuera de qemu, se compila el proyecto haciendo (en el root del proyecto):

```bash
make buddytest
```

y para ejecutar el ejecutable generado (se genera en la carpeta testing).

```bash
cd Testing
./buddyTest <memoryAmount>
```

o 

```bash
make bitmaptest
```

y para ejecutar el ejecutable generado (se genera en la carpeta testing).

```bash
cd Testing
./bitmapTest <memoryAmount>
```
## Limitaciones

### Memory managers
* Se usa parte de la memoria provista en la inicialización del memory manager para guardar estructuras auxiliares. Por este motivo la interfaz tiene métodos para obtener la memoria utilizable.
* Al inicio de la ejecución del kernel se le asignan 10000 bytes al memory manager.
