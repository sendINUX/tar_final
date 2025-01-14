#!/bin/sh
echo ""
echo ""
echo ""
echo "*******************************************************************************"
echo "******************** Creando los ficheros para las pruebas ********************"
echo "*******************************************************************************"
echo "hola caracola" > fichero1.txt 
mkdir directorioVacio
chmod +x directorioVacio/

mkdir directorioLleno 

echo "funciona" > ./directorioLleno/ficheroDentro.txt
echo "caracola" > ./directorioLleno/ficheroDentro2.txt
touch archivoVacio.txt
echo "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq" >fichero2.txt
echo "a este fichero le van a hacer un enlace tope de guapo" >  fichero3.txt
ln -s fichero3.txt enlace
echo ""
echo "******************** Compilando ambas utilidades... ********************"
gcc  ./lib/*.c ./tar_tools_insertar/*.c -o  insertar_fich 
gcc ./lib/*.c ./tar_tools_extraer/*.c -o  extraer_fich
echo ""



echo "*******************************************************************************"
echo "********************		PRUEBAS DE INSERTA_FICHERO       ********************"
echo "*******************************************************************************"

echo ""
echo ""
echo ""
echo "##########Caso de prueba 1: Ejecutar el programa con menos de 2 parámetros ##########"
echo "Resultado esperado: error por falta de argumentos al llamar a una función"
./insertar_fich
sleep 0.5
echo ""
echo ""
echo ""
echo "##########Caso de prueba 2: Ejecutar el programa con un fichero a comprimir que no existe ##########"
echo "Resultado esperado: error por no poder encontrar el fichero a añadir"
echo ""
./insertar_fich archivoInexistente.txt mytar.tar
sleep 0.5
echo ""
echo "mytar.tar NO debe existir. Lo comprobamos con ls..."
ls -la  mytar.tar
echo ""
echo ""
echo ""
echo "##########Caso de prueba 3: Ejecutar el programa con un fichero vacio ##########"
echo "Resultado esperado: se inserta solo el header del archivo"
echo ""
./insertar_fich archivoVacio.txt mytar.tar
sleep 0.5
tar -tvf mytar.tar
echo ""
echo ""
echo ""
echo "##########Caso de prueba 4: Ejecutar el programa con un archivo menor de 512 Bytes a insertar ##########"
echo "Resultado esperado: se inserta correctamente"
echo ""
./insertar_fich fichero1.txt mytar.tar
sleep 0.5
tar -tvf mytar.tar
echo ""
echo ""
echo ""
echo "##########Caso de prueba 5: Ejecutar el programa con un archivo mayor de 512 Bytes a insertar ##########"
echo "Resultado esperado: se inserta correctamente"
echo ""
./insertar_fich fichero2.txt mytar.tar
tar -tvf mytar.tar

echo ""
echo ""
echo ""
echo "##########Caso de prueba 6: Ejecutar el programa para insertar un enlace 	 ##########"
echo "Resultado esperado: se añade el enlace correctamente "
echo ""
./insertar_fich enlace mytar.tar 
tar -tvf mytar.tar

echo ""
echo ""
echo ""
echo "##########Caso de prueba 7: Ejecutar el programa con un directorio vacio	 ##########"
echo "Resultado esperado: se añade el directorio "
echo ""
./insertar_fich directorioVacio mytar.tar  
tar -tvf mytar.tar

echo ""
echo ""
echo ""
echo "##########Caso de prueba 8: Ejecutar el programa con un directorio con archivos	 ##########"
echo "Resultado esperado: se añade el directorio y su contenido"
echo ""
./insertar_fich directorioLleno mytar.tar  
sleep 0.5
tar -tvf mytar.tar


echo ""
echo ""
echo ""
echo "##########Caso de prueba 9: Ejecutar el programa con varios archivos con los permisos modificados	 ##########"
echo "Resultado esperado: Se añade el directorio, se almacenan los permisos tras la insercion"
echo ""
chmod 777 directorioLleno
chmod +x fichero2.txt
ls -la fichero2.txt
ls -la directorioLleno
./insertar_fich directorioLleno mytar2.tar  
echo ""
sleep 0.5
echo ""
echo "Insertado ahora fichero2.txt a mytar2.tar..."
echo ""
./insertar_fich fichero2.txt mytar2.tar  
tar -tvf mytar2.tar




echo ""
echo ""
echo ""
echo "##########Caso de prueba 10: Inserción de un archivo sobre el mismo (mismo fichero como parámetro f_dat y f_mytar)	 ##########"
echo "Resultado esperado: Detecta un uso incorrecto; reporta el error"
echo ""
sleep 0.5
./insertar_fich fichero2.txt fichero2.txt
sleep 0.5







echo ""
echo ""
echo ""
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo ""
echo ""
echo ""
echo "*******************************************************************************"
echo "********************		PRUEBAS DE EXTRAE_FICHERO        ********************"
echo "*******************************************************************************"
echo ""
echo ""
echo ""
echo "##########Caso de prueba 1: Ejecutar el programa con menos de 2 parámetros ##########"
echo "Resultado esperado: error por falta de argumentos al llamar a una función"
./extraer_fich
echo ""
echo ""
echo ""

echo "##########Caso de prueba 2: Ejecutar el programa con un fichero tar inexistente ##########"
echo "Resultado esperado: error por no poder encontrar el fichero tar"
./extraer_fich fichero1.txt tarFalso.tar
echo ""
echo ""
echo ""

echo "########## Caso de prueba 3: Ejecutar el programa con un fichero que no sea tar ##########"
echo "Resultado esperado: error por no poder abrir correctamene el archivo tar"
./extraer_fich fichero1.txt fichero2.txt



echo "########## Caso de prueba 4: Extraccion (desde un archivo tar valido) de un fichero regular de tamaño mayor que el bloque de datos 	 ##########"
echo "El resultado esperado sería: extrae el archivo (y su tamaño es mayor que 512 Bytes) "
echo "se va a borrar el archivo fichero2.txt para ver si se extrae correctamente..."
echo ""
rm fichero2.txt
./extraer_fich fichero2.txt mytar.tar
echo ""
ls -la fichero2.txt
echo ""





echo "########## Caso de prueba 5: Ejecutar el programa con un fichero que no se encuentre en el tar	 ##########"
echo "El resultado esperado sería: devuelve un error "
echo ""
./extraer_fich ficheroinexistente.txt mytar.tar 
echo ""
echo ""
echo ""


echo "########## Caso de prueba 6: Extraccion (desde un archivo tar valido) de un fichero regular de tamaño menor que el bloque de datos 	 ##########"
echo "El resultado esperado sería: extrae el archivo (y su tamaño es menor que 512 Bytes) "
echo "se va a borrar el archivo fichero1.txt para ver si se extrae correctamente..."
rm fichero1.txt
./extraer_fich fichero1.txt mytar.tar 
echo ""
ls -la fichero1.txt

echo ""
echo ""
echo ""
echo "########## Caso de prueba 7: Ejecutar el programa con un enlace	 ##########"
echo "El resultado esperado sería: extrae el enlace y se enlaza con el archivo "
rm enlace
./extraer_fich enlace mytar.tar 
echo "Comprobamos con: ls -la enlace"
echo ""
ls -la enlace

echo ""
echo ""
echo ""
echo "########## Caso de prueba 8: Ejecutar el programa con un directorio vacio ##########"
echo "El resultado esperado sería: extrae el directorio vacio "
echo "se va a eliminar el directorio previamente creado para ver si este se genera correctamente"
echo ""
rmdir directorioVacio
./extraer_fich directorioVacio mytar.tar 
echo ""
echo ""
echo ""

echo "########## Caso de prueba 9: Ejecutar el programa con un directorio lleno	 ##########"
echo "El resultado esperado sería: extrae el directorio entero "
echo "se va a directorio el archivo enlace para ver si se extrae correctamente"
echo ""
rm -rf directorioLleno
./extraer_fich directorioLleno mytar.tar 


echo ""
echo ""
echo ""
echo "########## Caso de prueba 10: Ejecutar el programa con archivos de mismo nombre para ver si se sobrescriben	 ##########"
echo "El resultado esperado sería: se sobrescriben y se ve en la pantalla la palabra funciona despues de hacer un cat al nuevo archivo "
echo ""
echo "esto debe sobreescribirse"> ./directorioLleno/ficheroDentro.txt
./extraer_fich directorioLleno mytar.tar 
cat ./directorioLleno/ficheroDentro.txt



echo ""
echo ""
echo ""
echo "########## Caso de prueba 11: Comprobacion de que la informacion de control (permisos) se almacena en el tar (sobreescritura de fichero regular, y extraccion de directorio previamente borrado)	 ##########"
echo "El resultado esperado sería: "
echo "El archivo fichero1.txt debería tener permisos de ejecución y el directorio directorioLleno debería tener todos activados "
echo ""
echo ""
echo "fichero1.txt inicialmente (tras ejecutar cambiara su info. de control):"
rm directorioLleno -rf
ls -la fichero1.txt
rm fichero1.txt
echo ""
echo "Ejecutando..."
echo ""
./extraer_fich directorioLleno mytar2.tar 
./extraer_fich fichero2.txt mytar2.tar 
echo "Comprobamos que funciono:"
ls -la directorioLleno
ls -la fichero2.txt
echo ""
echo ""
echo "Limpiando... "
echo "Borrando archivos de prueba y binarios creados..."
rm -rf enlace *.txt *.tar *_fich directorioLleno directorioVacio












