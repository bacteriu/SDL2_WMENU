Una clase echa en SDL2 para añadir Menús a una ventana.
---------------------------------------------------------------------------------------------------
Para utilizarla copia el archivo WMenu.cpp a tu directorio source y WMenu.h a tu directorio include.
Una vez echo esto añade el fichero WMenu.cpp y WMenu.h a tu projecto y despues declare una variable de tipo WMenu del siguiente modo:
WMenu var(SDL_Window* wID,const char* FName,uint32_t h,uint32_t tc, uint32_t mc, uint32_t lc, uint32_t dc, uint32_t type=0);
Los parametros de la declaración son los siguientes:
  wID: un identificador de la ventana obtenido con cuando se creó con la función SDL_CreateWindow()
  FName: el nombre del font que utilizarán los menus el font debe contener el path donde esté hubicado.
  h: Sera la altura en pixeles de la barra de menu, hay que tener en cuenta que el tamaño del font se obtendrá como h/1.75
  tc: Color del texto del menu.
  mc: Color de la barra de menu y el fondo de todos los items.
  lc: Color claro que se utilizará para el contorno de la barra de menu.
  dc: Color oscuro que tambien se utilizará para el contorno de la bara de menu.
  type: Tipo del marco del menu 0 para 3D y 1 para 2D con sombra.
Tras declarar una o varias variables de tipo WMenu podras utilizar las siguientes funciiones:
  - int32_t addMenu(const char name[]);
      Esta función añade un menu a una barra de Menu en la parte superior de la ventana, name[] es el nombre del menu, retorna el indice del menu en la barra de menu
  - int32_t addItem(int32_t parentitem, const char name[],int32_t item, int32_t type=1,SDL_Surface* sfc=NULL);
      Esta función añade un item con el nombre name[] al menu seleccionado con el parametro parentitem, item es el numero identificador que se usará posteriormente para detectar eventos sobre este item, este valor puede ser WMENU_ITEM_ANY en ese caso la función buscará un identificador libre que sera devuelto por la función, type indica si el item puede ser WM_DEFAULT, WM_TOGLE y WM_IMAGE si es normal solo se muestra el nombre del item si es de tipo WM_TOGLE mostrará un rectangulo que puede alternarse con un tick y si es de tipo WM_IMAGE mostrará una imagen delante del nombre en este caso habrá que pasar el surface de la imagen en el ultimo parametro sfc.
  - int32_t insItem(int32_t parentitem, const char name[],int32_t item, int32_t type=1, SDL_Surface* sfc=NULL);
      Esta funcion es igual que la anterior solo que en lugar de colocar el item en la ultima posición del parentitem solicitado lo colocará delante del parentitem solicitado.
  - int32_t insMenu(int32_t index, const char name[]);
      Esta función es igual a addMenu() pero inserta un menu en la posición seleccionado por el parametro index si existe y si no encuentra el index lo añade a la derecha del menu anterior.
  - int32_t rmEndItem(int32_t item);
      Esta función elimina un item terminal si el parametro item es encontrado, el item seleccionado debe no contener item colgando de el. Devuleve 1 si se encontrol el item y era un item terminal y 0 si no lo encuentra o eraa un item terminal.
  - int32_t delItems(int32_t item);
      Esta función elimina un item y todos los subitems que cuelguen de el. Devuelve el numero total de items eliminados o 0 si no encuentra el item solicitado.
  - int32_t delMenu(int32_t index);
      Esta funcion elimina un menu de la barra de menu selecionado por el parametro index, devuelve 1 si lo ha eliminado y 0 si no se encontro el menu index.
  - SDL_Surface* setItemImage(int32_t item,SDL_Surface* sfc);
      Esta función cambia el surface de la imagen de un item de tipo WM_IMAGE, los parametros de entrada son el item que será buscado y sfc que es la superficie que le asignará al item, Devuelve el surface que tubiera ese item si se encuentra o NULL si no tenia ningun surface o no se encontrol el item. Esta función ni ninguna otra libera el surface asignado a un item por lo que usted deberá hacerlo cuando corresponda.
  - int getBarHeight();
      Esta función retorna la altura en pixeles de la barra de menu.
  - void setAble(int32_t item, bool st);
			Esta función habilita o desabilita un item de un menu segun el parametro de entrada st, si se establece como false el item aparecera sombreado en el menu y se desplegarán los subitem que cuelguen a su derecha si los hay.
  -	int32_t getAble(int32_t item);
			Esta función devuelve el estado del item solicitado si lo encuentra, si el item esta enable devuelve 1, si esta disable devuelve 0, y si el item no se encuentra devuelve -1.
  -	void setData(int32_t item, int data);
			Esta función estable el valor del campo data de la estructura Menu_t, el bit mas bajo es usado para los item de tipo WM_TOGLE para indicar si el tendran el tick activado o no.
  -	int32_t getData(int32_t item);
			Esta función devuelve el valor del campo data de la estructura Menu_t, el bit mas bajo de este campo es usado para los item de tipo WM_TOGLE indicando si tiene el tick activado o no.
  -	int32_t setItemTitle(const char* ntitle, int32_t item);
			Esta función estable el titulo o nombre de un item segun el parametro de entrada ntitle, y el item indicado por el parametro de entrada item. La maxima longitud del campo sera la indicada la constante MENU_MAX_LENG=64, si el titulo es de una longitud mas larga solo se copiaran los primeros 62 caracteres y los dos ultimos seran ".." para indicar que el titulo es mas largo. Esta función devuelve el numero de caracteres copiados al titulo del item si se encuentra, si el item indicado no se encuentra devuelve -1.
  -	void setItemCallBAck(int32_t item, void (* calback)(Menu_t*));
			Esta función establece el la función a ejecutar si un item terminal es picado, esto puede usarse para ejecutar funciones automaticamente cuando se pica un item de un menu.
  -	void drawMenu();
			Esta función dibuja el menu en la ventana, esta función solo dibuja el menu de la instancia utilizada en la llamada es decir si uste realizó la declaración como WMenu var(. . .) la llamada a esta función como var.drawMenu() solo dibujará el menu var para dibujar todos los menus que haya declarado utilize la función fuera de clase WMenu_Update().
	
	Función fuera de clase.
  -	void WMenu_Update();
			Esta función dibuja todas las instancias de menu que haya declarado para lo cual recorre todas las instancias existentes y llamara a la función drawMenu() correspondiente a cada instancia.
	
---------------------------------------------------------------------------------------------------	
	Advertencias:
	He de decir que esta libreria la he realizado para mi uso personal y dada mi inexperiencia programando no hay ninguna garantia de que funcione correctamente en todos los casos.
	Ademas, por esta misma razon seguramente estará llena de bug y falta de obtimizaciónes y seguro que se puede hacer mucho mejor aunque para mi ha sido suficiente, sientase libre de mejorarla o simplemente utilícela para aprender un poco sobre eventos yo he aprendido y disfrutado mucho haciendolo.
	Adjunto tambien un pequeño ejemplo que muestra como utilizarla.
	
