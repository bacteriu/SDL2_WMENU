## Una clase hecha en SDL2 para añadir Menús a una ventana.
---------------------------------------------------------------------------------------------------
Para utilizarla tendrás que haber configurado tu compilador para poder usar las librerías SDL, SDL_IMAGE y SDL_TTF en tu proyecto, yo he seguido las instrucciones de "Lazy Foo'" en el siguiente enlace:
	https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php
 
Una vez que puedas compilar usando las librerías SDL copia el archivo WMenu.cpp a tu directorio source y WMenu.h a tu directorio include.
Una vez hecho esto añade el fichero WMenu.cpp y WMenu.h a tu proyecto y después declare una variable de tipo WMenu del siguiente modo:

WMenu var(SDL_Window* wID,const char* FName,uint32_t h,uint32_t tc, uint32_t mc, uint32_t lc, uint32_t dc, uint32_t type=0);

Los parámetros de la declaración son los siguientes:


	wID: un identificador de la ventana obtenido con cuando se creó con la función SDL_CreateWindow().
	FName: el nombre del font que utilizarán los menús el font debe contener el path donde esté ubicado.
	h: Sera la altura en pixeles de la barra de menú, hay que tener en cuenta que el tamaño del font se obtendrá como h/1.75.
	tac: Color del texto del menú.
	mc: Color de la barra de menú y el fondo de todos los ítems.
	lc: Color claro que se utilizará para el contorno de la barra de menú.
	dc: Color oscuro que también se utilizará para el contorno de la barra de menú.
	type: Tipo del marco del menú 0 para 3D y 1 para 2D con sombra.
 
Tras declarar una o varias variables de tipo WMenu podrás utilizar las siguientes funciones:
  -	int32_t addMenu(const char name[]);
			Esta función añade un menú a una barra de Menú en la parte superior de la ventana, name[] es el nombre del menú, retorna el índice del menú en la barra de menú
  -	int32_t addItem(int32_t parentitem, const char name[],int32_t item, int32_t type=1,SDL_Surface* sfc=NULL);
			Esta función añade un ítem con el nombre name[] al menú seleccionado con el parámetro parentitem, ítem es el numero identificador que se usará posteriormente para detectar eventos sobre este ítem, este valor puede ser WMENU_ITEM_ANY en ese caso la función buscará un identificador libre que será devuelto por la función, type indica si el ítem puede ser WM_DEFAULT, WM_TOGLE y WM_IMAGE si es normal solo se muestra el nombre del ítem si es de tipo WM_TOGLE mostrará un rectángulo que puede alternarse con un tic y si es de tipo WM_IMAGE mostrará una imagen delante del nombre en este caso habrá que pasar el surface de la imagen en el último parámetro sfc.
  -	int32_t insItem(int32_t parentitem, const char name[],int32_t item, int32_t type=1, SDL_Surface* sfc=NULL);
			Esta función es igual que la anterior solo que en lugar de colocar el ítem en la última posición del parentitem solicitado lo colocará delante del parentitem solicitado.
  -	int32_t insMenu(int32_t index, const char name[]);
			Esta función es igual a addMenu() pero inserta un menú en la posición seleccionado por el parámetro índex si existe y si no encuentra el índex lo añade a la derecha del menú anterior.
  -	int32_t rmEndItem(int32_t item);
			Esta función elimina un ítem terminal si el parámetro ítem es encontrado, el ítem seleccionado no debe contener ítem colgando de él. Devuelve 1 si se encontró el ítem y era un ítem terminal y 0 si no lo encuentra o no era un ítem terminal.
  -	int32_t delItems(int32_t item);
			Esta función elimina un ítem y todos los subitems que cuelguen de él. Devuelve el número total de ítems eliminados o 0 si no encuentra el ítem solicitado.
  -	int32_t delMenu(int32_t index);
			Esta función elimina un menú de la barra de menú seleccionado por el parámetro índex, devuelve 1 si lo ha eliminado y 0 si no se encontró el menú índex.
  -	SDL_Surface* setItemImage(int32_t item,SDL_Surface* sfc);
			Esta función cambia el surface de la imagen de un ítem de tipo WM_IMAGE, los parámetros de entrada son el ítem que será buscado y sfc que es la superficie que le asignará al ítem, Devuelve el surface que tuviera ese ítem si se encuentra o NULL si no tenía ningún surface o no se encontró el ítem. Esta función ni ninguna otra libera el surface asignado a un ítem por lo que usted deberá hacerlo cuando corresponda.
  -	int32_t getBarHeight();
			Esta función retorna la altura en pixeles de la barra de menú.
  -	void setAble(int32_t item, bool st);
			Esta función habilita o deshabilita un ítem de un menú según el parámetro de entrada st, si se establece como false el ítem aparecerá sombreado en el menú y se desplegarán los subitem que cuelguen a su derecha si los hay.
  -	int32_t getAble(int32_t item);
			Esta función devuelve el estado del ítem solicitado si lo encuentra, si el ítem está habilitado devuelve 1, si esta deshabilitado devuelve 0, y si el ítem no se encuentra devuelve -1.
  -	bool setData(int32_t item, int data);
			Esta función estable el valor del campo data de la estructura Menu_t, el bit más bajo es usado para los ítems de tipo WM_TOGLE para indicar si el ítem tendrá el tic activado o no. Retorna true si se encuentra ítem y false si no se encuentra.
  -	bool getData(int32_t item, int &data);
			Esta función devuelve el valor del campo data de la estructura Menu_t en el parámetro de salida data, el bit más bajo de este campo es usado para los ítem de tipo WM_TOGLE indicando si tiene el tic activado o no. Retorna true si el ítem es encontrado y el parámetro de salida data es válido y false si ítem no es encontrado en este caso data contendrá el valor previo a la llamada a esta función.
  -	int32_t setItemTitle(const char* ntitle, int32_t item);
			Esta función estable el titulo o nombre de un ítem según el parámetro de entrada ntitle, y el ítem indicado por el parámetro de entrada ítem. La máxima longitud del campo será la indicada la constante MENU_MAX_LENG=64, si el título es de una longitud más larga solo se copiarán los primeros 62 caracteres y los dos últimos serán ".." para indicar que el título es más largo. Esta función devuelve el número de caracteres copiados al título del ítem si se encuentra, si el ítem indicado no se encuentra devuelve -1.
  -	int32_t getItemTitle(int32_t item, char* buftitle, int32_t leng);
			Esta función obtiene una copia del título de un ítem en buffer especificado por el parámetro de entrada buftitle con una longitud máxima de leng caracteres. Retorna la cantidad de caracteres copiados en buftitle o 0 en el caso de que buftitle=NULL, ítem no sea encontrado o leng sea igual a 0.
  -	int32_t getItemType(int32_t item); Esta función retorna el tipo de un ítem o -1 si el ítem no ha sido encontrado. Los tipos de ítems pueden ser 0 para menú de la barra de menú, 1 para ítems de tipo WM_DEFAULT, 2 para ítems de tipo WM_TOGLE y 4 para ítems de tipo WM_IMAGE.
  -	void setItemCallBAck(int32_t item, void (* calback)(Menu_t*));
			Esta función establece la función a ejecutar si un ítem terminal es picado, esto puede usarse para ejecutar funciones automáticamente cuando se pica un ítem de un menú.
  -	void drawMenu();
			Esta función dibuja el menú en la ventana, esta función solo dibuja el menú de la instancia utilizada en la llamada es decir si usted realizó la declaración como WMenu var(. . .) la llamada a esta función como var.drawMenu() solo dibujará el menú var, para dibujar todos los menús que haya declarado utilice la función fuera de clase WMenu_Update().
---------------------------------------------------------------------------------------------------
Función fuera de clase.
  -	void WMenu_Update();
			Esta función dibuja todas las instancias de menú que haya declarado para lo cual recorre todas las instancias existentes y llamara a la función drawMenu() correspondiente a cada instancia.

---------------------------------------------------------------------------------------------------
Advertencias:

He de decir que esta librería la he realizado para mi uso personal y dada mi inexperiencia programando no hay ninguna garantía de que funcione correctamente en todos los casos.
 Además, por esta misma razón seguramente estará llena de bug y falta de optimizaciones y seguro que se puede hacer mucho mejor, aunque para mí ha sido suficiente, siéntase libre de mejorarla o simplemente utilícela para aprender un poco sobre eventos SDL2, yo he aprendido y disfrutado mucho haciéndolo.
 
 Adjunto también un pequeño ejemplo que muestra cómo utilizarla.
 
![Image_0009](https://github.com/bacteriu/SDL2_WMENU/assets/66980919/6b8be19f-7fe6-4fe1-91b6-6bb97191a8d9)

![Image_0011](https://github.com/bacteriu/SDL2_WMENU/assets/66980919/3381b86b-6b9b-44d3-b9ed-f3493be5c1fd)

¿Que falta por hacer?:

Faltan muchísimas cosas que poco a poco terminaré implementando, al principio del fichero WMenu.cpp hay una lista de todas las funciones y opciones que me gustaría añadir algún día. Pero de momento he de darle uso para ver todos los posibles bugs que pueda observar para corregirlos, así como la posible variación de las funciones actuales y futuras que puedan ser necesarias para un cómodo uso de la librería.
