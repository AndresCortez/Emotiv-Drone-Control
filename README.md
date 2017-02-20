# Emotiv-Drone-Control
Control de un Dron Ar-Parrot 2.0

# Puntos a tomar en consideración antes de correr los programas
-Todo el proyecto ha sido realizado en la version de Visual Estudio 2015 por lo que puede existir errores de compatibilidad con otras versiones
Para poder realizar el proyecto es necesario que cuentes con la edicion de investigacion de Emotiv Emotiv Research Edition SDK_v1.0.0.5-PREMIUM. Puedes entrar a la página oficial de Emotiv para más información https://www.emotiv.com/
-Es necesario que la carpeta VS 2015 se encuentre en la siguiente dirección para que pueda funcionar el programa C:\Program Files (x86)\Emotiv Research Edition SDK_v1.0.0.5-PREMIUM\EmoScript Samples\doc\examples\example6
-La carpeta VS 2015 contiene el proyecto de Visual Estudio del manejo del dron 
-Asegurate que el archivo de texto que se cree para comunicar los dos programas se encuentre donde este el proyecto GyroData
-Es recomendable trabajar en las carpetas ya antes mencionadas ya que en muchos casos no corrian los programas por que se encontraban en direcciones diferentes




# Pasos para Correr el proyecto 
- Luego de haber descargado los archivos de emotiv necesitas correr el VS project llamado GyroData que se encuentra en la siguiente carpeta 
C:\Program Files (x86)\Emotiv Research Edition SDK_v1.0.0.5-PREMIUM\EmoScript Samples\doc\examples\example6
- En VS en el proyecto de GyroData modificar el codigo de main.cpp por el que se encuentra en el repositorio. 
- Para que funcione el control del dron junto a la diadema se necesitan correr los dos proyectos al mismo tiempo es decir GyroData y Test la forma que encontré para resolver esto es Visual Studio en la parte superior derecha va aparecer una opcion que se encuentra como "Solution 'GyroData' (1 project)" en esta opción le damos click derecho y seleccionamos "add" y luego "Existing project" mandamos a llamar al proyecto test ques es el encargado de controlar el dron este proyecto debe encontrarse en la siguiente direccion C:\Program Files (x86)\Emotiv Research Edition SDK_v1.0.0.5-PREMIUM\EmoScript Samples\doc\examples\example6\vs2015  luego en la barra de herramientas en Project elegimos la opcion Set Startup Projects, seleccionamos Multiple startup projects y en los dos proyectos  tiene que estar en start esto hará que los dos proyectos corran al mismo tiempo 
- En el proyecto test tenemos que modificar el archivo drone.cpp por el que se encuentra en el repositorio. 


 
