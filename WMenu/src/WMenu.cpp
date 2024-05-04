/*******************************************************************************************************
 * TODO: Fix bug, cuando picas en un menu que esta sobre el figurero es como si picaras fuera del menu
 *       en el figurero.
 *      (Echo), Solucion en (V022): Set moverptr to null only when barpiked=false && moverptr->type>0
 *
 * TODO: En AddMenu/submenu falta decir que los id de la barra de menus se incrementan
 *      automaticamente pero los item de los submenus los indicamos nosotros por lo que si
 *      añadimos un submenu con un numero y luego añadimos un menu se produce
 *      un conflicto ya que el id del menu se incrementa automaticamente pudiendo
 *      coincidir con un submenu que hayamos añadido anteriormente.
 *      (Echo) Solución: no es necesario cambiar nada: Si coincide un item con un menu en la barra
 *                       no es ningun problema ya que solo los item que no están en la barra generaran
 *                       un evento con su id por lo que los Ids de la barra solo sirven para colgar
 *                       submenus debajo.
 *
 * TODO: Obtimizar el dibujado del menu drawall usando recursividad o algo
 *       (Echo) aunque sin usar recursividad.
 *
 * TODO: En la clase WMenu Cambiar setMenuSize por setSurface y copiar la w,h del surface
 *      (Echo) ahora el creador de WMenu recibe el Window Id y obtiene el surface correspondiente
 *             en el filtro de eventos se resizea el menu al cambiar  el tamaño dela ventana
 *
 * TODO: Separar la clase WMenu en otro fichero .cpp & .h para poder reutilizar en otros programas
 *       (Echo) (V026) se elimina la clase WMenu del programa principal pasando a utilizar WMenu.h
 *               y WMenu.cpp.
 *
 * TODO: Cuando hay varios WMenu determinar cual tiene barpiked=true y pintar los demas en primer lugar
 *       (Echo) Solucion (V025): Se añaden la función WMenu_Update() que llama las funciones necesarias
 *              para dibujar los menus en orden correcto.
 *
 * TODO: Añadir a la Clase WMenu un ptr a la primera instancia de WMenu creado y propagarlo por todas
 *       las instancias para poder acceder en orden ascendente sin tener que recorrer la lista de WMenu
 *       en orden inverso hasta llegar al primero, esto implica tener una lista bidireccional de WMenu
 *       añadiendo un firWMenuInstance, nextWMenuInstance y previousWMenuInstace
 *       (Echo) Solucion (V024): Implementado, se añade tambien un puntero estatico apuntado siempre
 *              a la primera instancia que haya en la lista.
 *
 * TODO: Cambiar la implementación de la pila push y pop menu item para usar una  lista en lugar de array
 *       (Echo) Solucion (V026): Implementado.
 *
 * TODO: Añadir dos ptr a WMenu para añadir EventFilter externo de modo que la aplicacion pueda
 *       añadir EventFilter propios y gestionarlos sin eliminar nuestra función EventFilter
 *       (Añadido pero aun no implementado)
 *
 * TODO: Añadir mas tipos de item que permitan tener un toglebox de selección y deselección
 *       (+-Echo) (V026): Añadido tipo 2 o WM_TOGLE que permite tener un check signal on/off
 *
 * TODO: Fix warning no return statement en la funcion chkmouseover.
 *       (Echo) Solucion (V024): Cambio esta funcion a void ya que no necesito debolución.
 *
 * TODO: Modificar la funcion drawdown para utilizar un surface ya creado al añadir item en
 *       lugar de dibujar el marco cada vez que se llama a dradown
 *       (ECHO) (V030): Modificadas las funciones drawdownmenu, para usar SDL_BlitSurface y
 *       (29/04/2024)   modificar drawframe para recibir un surface lo que hace inecesario
 *                      recibir smbox ya que solo necesitamos w y h para dibujar en el surface.
 *
 * TODO: Gestionar la opción del tipo de marco o incluso añadir CALLBACK para dibujar con una
 *       funcion definida por el usuario.
 *       (Echo parcialmente) (V26):tipo 0 y 1 son ahora correctamente dibujados falta CallBack
 *
 * TODO: Añadir funciones para cambiar title, insertar y eliminar item con/sin sus descendientes.
 *       (Echo) (V026): Añadida función setItemTitle, insMenuItem y rmMenuItem
 * TODO: Añadir mismas funciones para los Item de la barra de menus y los subitems que cuelgue de
 *       de otro item
 *
 * TODO: Gestionar un ITEM_ANY, si llamamos a cualquier funcion que añade un item con numero de
 *       de item= (INT32_T-1) es decir el mayor numero negativo posible definiendolo como una
 *       constante como WM_ANY o algo así, para que dichas funciones busquen un item no utilzado
 *       y lo devuelva en el retorno de la funcion.
 *       (ECHO) (V028): Añadida constante WMENU_ITEM_ANY=int32_t -1, pasando este valor como item
 *                      se buscará un item ID no utilizado y se devolverá en la función addsubmenu
 *
 * TODO: Forzar que la función item CallBack devuelva un integer con valor 1 o 0 para filtrar el
 *       evento, si devuelve 1 => despues del CallBack se injectará un evento del Item, si devuelve
 *       0 no se injectará evento y el programa principal no recibira el evento, en este caso la
 *       funcion myEventFilterFuntion gestionará los check box mientras que si la función callback
 *       ha devuelto 1 es esta función la que debe gestionar el checkbox.
 *
 * TODO: Cambiar nombre a las funciones rmItem, rdelallitem, rmMenuItem, delSubItems para adecuarse
 *       mejor a lo que hacen.
 *
 * TODO: Averiguar porque event.user.data2 pierde el dato de la instancia parece un bug de SDL2
 *
 * TODO: Modificar findFreeItem() para que encuentre un item unico incluso para todas las instancias.
 *
 * (V026): Añadido setItemCallBAck(item, callback(Menu_t*)) lo que permite ejecutar funciones para
 *         cada item independientemente de que genere un evento o no
 * (V026): Añadido setAble(item, false/true), permite habilitar o desabilitar un item
 * (V026): Añadido getAble(item), devuelve si un item esta desabilitado o no
 * (V026): SetData(int32_t item, int data), assigna el valor data al item menu->data
 * (V026): Añadido shadown box alrededor del check en los item type WM_TOGLE
 * (V027): Eliminada la opción de utilizar pop y push con array dejando solo la opción de una lista
 * (V027): Modificada la función adjsmbox para permitir modificar tambien los item de la barra de menú
 * (V028): Añadida función delSubItems(item) que elimina cualquier item incluyendo sus subitem
 * (V028): Cambiado el nombre las siguientes funciones: addsubmenu=additem,
 ******************************************************************************************************
 * Versión: V0.030
 ******************************************************************************************************/
#include "WMenu.h"
#ifndef __DEBUG
    #define __DEBUG
#endif // __DEBUG



WMenu::WMenu(SDL_Window* wID,const char* FName,uint32_t h,uint32_t tc, uint32_t mc, uint32_t lc, uint32_t dc, uint32_t type) {  //Constructor
    if(wID==NULL) {
        printf("(WMenu Constructor) Error Windows handled is NULL");
        exit(1);
    }
    windowID=wID;
    //Seting color & other seting
    tcolor=tc;  //Text color
    mcolor=mc;  //Menu color;
    lcolor=lc;  //Light color;
    dcolor=dc;  //Dark color;
    scolor=0xFFE0E0FF;
    pcolor=0xFFA0A0FF;
    WMType=type;

    //Seting wSurface from windows ID
    wSurface = SDL_GetWindowSurface(wID);
    if(wSurface==NULL) {
        printf("(WMenu Constructor) Error getting surface from windows operand");
        exit(2);
    }

    //localEventFilter = ptr a la funcion que realiza el filtro =myEventFilterFuntion
    //localUserDataPtr = userdata ptr a pasar a la funcion EventFilter
    //LLegados a este punto obtendremos el filtro anterior si lo hay
    // por lo que pueden ocurrir 4 cosas:
    // 1ª No hay ningun filtro previo, en este caso registramos nuestro
    //      UserEventType y despues añadiremos el filtro correspondiente
    // 2º Hay filtro previo pero no es de la Class WMenu en este caso
    //      guardamos el correspondiente lastEventFilter y lastInstanePtr   //<- TODO:
    //      e igualmente registramos nuestro UserEventType y su filtro
    // 3º Hay filtro previo y ademas es de la Clase Wmenu hacemos =>
    //      guardamos el correspondiente lastEventFilter y lastinstancePtr
    //      ademas assignaremos instance= lastInstancePTR->instance + 1;
    //
    // 4º No hay ningun filtro previo y ademas es de la Clase WMenu =>
    //      Esto no es posible
    //Reserve on event user type for this menu
    void*               localUserDataPtr=NULL;
    SDL_EventFilter     localEventFilter=0;
    if(!SDL_GetEventFilter(&localEventFilter,&localUserDataPtr)) {  //No filtro previo
        firstInstancePtr=(WMenu*)this;
        FirstInstance=(WMenu*)this;
        myEventType = SDL_RegisterEvents(1);
    } else if (localEventFilter!= &myEventFilterFuntion){                 //Hay filtro previo pero o es de la clase WMenu
        firstInstancePtr=(WMenu*)this;
        FirstInstance=(WMenu*)this;
        myEventType = SDL_RegisterEvents(1);
        otherInstancePtr=localUserDataPtr;
        otherEventFilter=localEventFilter;
    } else if (localEventFilter == &myEventFilterFuntion) {               //Hay filtro previo Y es de la clase WMenu
        while (((WMenu*)localUserDataPtr)->nextInstancePtr!=NULL)
            localUserDataPtr=((WMenu*)localUserDataPtr)->nextInstancePtr;     // Get the last instance
        lastInstancePtr=(WMenu*)localUserDataPtr;                   // Save current lastInstancePtr with previo Instance
        firstInstancePtr=lastInstancePtr->firstInstancePtr;         // Copy firstInstance to current Instance
        lastInstancePtr->nextInstancePtr=(WMenu*)this;              // Set previous instance nextInstance with current instance (this)
        myEventType = lastInstancePtr->myEventType;                 // Copy myEventType of lastinstance to currentInstance
        instance=lastInstancePtr->instance+1;                       // Inc current instance count
    }

    //Con nuestro userEventType registrado establecemos los campos minimos y colocamos nuestra funcion como filtro para los eventos
    if (myEventType==(Uint32)-1) {
        printf("Error.. no se ha podido registrar los eventos para el menu...");
        exit(3);
    } else {
        MenuEvent.user.type=myEventType;
        SDL_SetEventFilter(&myEventFilterFuntion,firstInstancePtr);   //this);
    }

    //Seting barbox of menu
    barbox.x=0;
    barbox.w=wSurface->w;
    barbox.h=h;
    barSurface=SDL_CreateRGBSurfaceWithFormat(0,barbox.w,barbox.h,32,SDL_PIXELFORMAT_ARGB8888);
    drawframe(barSurface,mcolor,lcolor,dcolor,thick,type);
    barbox.y=(lastInstancePtr!=NULL)? (lastInstancePtr->barbox.y+lastInstancePtr->barbox.h):0;
    //Seting font style & init and load font
    fontstyle=TTF_STYLE_BOLD;
    if (fontid==NULL && TTF_Init()==-1) {
        printf("(WMenu Constructor) Error initializing TTF library\n\r");
        exit(3);
    }
    fontid=TTF_OpenFont(FName,barbox.h/1.75);    // "CAMBRIA.ttc", Fsize );
    if (fontid==NULL) {
        printf("Font loading Error...\n\r");
        exit(2);
    }
    TTF_SetFontStyle(fontid,fontstyle);

    //Seting ItemHeight and draw frame of menu barbox
    itemheight=TTF_FontHeight(fontid);

    //Guardamos el tamaño del caracter > para usarlo en el calculo de los menus
    arrowwith=gettxtwith(">");
    //Incrementamos el numero total de instancias
    instances++;
}


int WMenu::gettxtwith(const char* txt) {
    int th,w;
    TTF_SizeText(fontid,txt,&w,&th);
    return w;
}
/********************************************************************
 * Dibuja un marco en el interior del parametro de entrada surface  *
 * Actualmente el marco puede ser de tipo 3D o Plano con sombra     *
 * Parametros:                                                      *
 *            surface: La superficie donde se pintara el marco      *
 *            mc:      Color del menu zona frontal                  *
 *            lc:      Color mas iluminado normalmente las lineas   *
 *                     Superior horizontal y lateral izquierda      *
 *            dc:      Color oscuro normalmente las lineas inferior *
 *                     horizontal y vertical derecha para el 3D y   *
 *                     contorno y sombra para el plano con sombra   *
 * Esta funcion no retorna nada.
 ********************************************************************/
void WMenu::drawframe(SDL_Surface* surface, uint32_t mc, uint32_t lc, uint32_t dc, uint32_t tk, int type) {
    SDL_Rect lbox={0,0,surface->w,surface->h};   //*box;
    uint32_t ddc=((dc/2)&0xFF000000) | (((dc&0xFF0000)/2)&0xFF0000) | (((dc&0xFF00)/2)&0xFF00) | (((dc&0xFF)/2)&0xFF);
    if (type>0) {
        lbox.h=1;
        lbox.x+=thick-1;
        lbox.w-=(thick-1)*2;
        SDL_FillRect(surface,&lbox,dc);        //Draw linea horizontal superior
        lbox.y+=surface->h-thick;
        SDL_FillRect(surface,&lbox,dc);        //Draw linea horizontal inferior
        lbox.w=1;lbox.y=0;lbox.h=surface->h-(thick-1);   //-tk;
        SDL_FillRect(surface,&lbox,dc);         //Draw linea vertical izquierda
        lbox.x= surface->w - thick;
        SDL_FillRect(surface,&lbox,dc);         //Draw linea vertical derecha
        lbox.x+=1; lbox.y=thick;lbox.w=thick;lbox.h=surface->h-thick;
        SDL_FillRect(surface,&lbox,ddc);         //Draw sombra vertical derecha
        lbox.x=thick*2; lbox.y=surface->h-thick+1; lbox.h=thick; lbox.w=surface->w-thick*2;
        SDL_FillRect(surface,&lbox,ddc);         //Draw sombra horizontal inferior
        lbox.x=lbox.y=0;lbox.w=surface->w;lbox.h=surface->h;
        lbox.x=thick;lbox.w=surface->w-tk*2;lbox.y=1;lbox.h=surface->h-tk-1;
        SDL_FillRect(surface,&lbox,mc);         //Draw menu frame box
    } else {
        lbox.x=lbox.y=0;lbox.w=surface->w;lbox.h=surface->h;    //Reset original values
        lbox.x+=tk;lbox.w=surface->w-tk*2;
        lbox.y+=tk;lbox.h=1;
        while (lbox.x>=0) {
            SDL_FillRect(surface,&lbox,lc);     //Draw marco horizontal superior
            lbox.x--;
            lbox.y--;
            lbox.w+=2;
        }

        lbox.x=tk;lbox.w=1;
        lbox.y=tk;lbox.h=surface->h-tk*2;
        while (lbox.x>=0) {
            SDL_FillRect(surface,&lbox,lc);     //Draw marco vertical izquierdo
            lbox.x--;
            lbox.y--;
            lbox.h+=2;
        }

        lbox.x=surface->w-tk;lbox.w=1;
        lbox.y=tk;lbox.h=surface->h-tk*2;
        while (lbox.y > 0) {
            SDL_FillRect(surface,&lbox,dc);     //Draw marco vertical derecho
            lbox.x++;
            lbox.h+=2;
            lbox.y--;
        }
        lbox.x=tk;lbox.w=surface->w-tk*2+1;
        lbox.y=surface->h-tk;lbox.h=1;
        while (lbox.x>0) {
            SDL_FillRect(surface,&lbox,dc);     //Draw marco horizontal inferior
            lbox.x--;
            lbox.y++;
            lbox.w+=2;
        }
        lbox.x=tk; lbox.w=surface->w-tk*2;
        lbox.y=tk; lbox.h=surface->h-tk*2;
        SDL_FillRect(surface,&lbox,mc);     //Draw menu box
    }
}

/********************************************************************************
 * Añade una opción a la barra del menu.                                        *
 * Parametros:                                                                  *
 *  name: El nombre que aparecerá en la barra del menu                          *
 *  item: Un identificador unico para posteriormente poder identificar el menu  *
 *        picado, si el identificador no existe o es -1 se añade a la derecha   *
 *        de la opción anterior.                                                *
 *  retorna: El identificador si era unico y se ha añadido la opción en el menú *
 *        o -1 si el identificador no era unico y no se ha añadido al menú      *
 * Usa: getlastItem() encuentra el ultimo item de la barra para añadir este item*
 ********************************************************************************/
int32_t WMenu::addMenu(const char name[]){
        Menu_t* current=(Menu_t*)calloc(1,sizeof(Menu_t));
        Menu_t* ptr=NULL;
        if (current!=NULL) {
            memset(current,0,sizeof(Menu_t));
        } else {
            printf("Error... Not enouft memory to create new menu structure");
            return -1;
        }
    #ifdef __DEBUG
        totalitem++;
    #endif // __DEBUG
        strcpy(name,current->mtitle,maxleng);
        current->enable=true;
        current->box.x=thick;
        current->box.y=barbox.y;
        current->box.w=gettxtwith(current->mtitle);
        current->box.h=barbox.h-thick*2;
        current->box.y+=thick;

        if (firstmenu==NULL) {
            firstmenu=current;              //if is first menu
        } else  {
            ptr=getlastmenu();             //get lastitem of barmenu type

            current->left=ptr;
            ptr->right=current;
            current->item=(ptr->item)+1;
            current->showed=false;
            current->box.x=ptr->box.x+ptr->box.w+SPACE;
        }
        return current->item;
};

/********************************************************************************
 * Añade una opción a la barra del menu.                                        *
 * Parametros:                                                                  *
 *  name: El nombre que aparecerá en la barra del menu                          *
 *  item: Un identificador unico para posteriormente poder identificar el menu  *
 *        picado, si el identificador no existe o es -1 se añade a la derecha   *
 *        de la opción anterior.                                                *
 *  retorna: El identificador si era unico y se ha añadido la opción en el menú *
 *        o -1 si el identificador no era unico y no se ha añadido al menú      *
 * Usa: getlastItem() encuentra el ultimo item de la barra para añadir este item*
 ********************************************************************************/
int32_t WMenu::insMenu(int32_t index, const char name[]){
        Menu_t* current=(Menu_t*)calloc(1,sizeof(Menu_t));
        Menu_t* ptr=NULL;
        if (current!=NULL) {
            memset(current,0,sizeof(Menu_t));
        } else {
            printf("Error... Not enouft memory to create new menu structure");
            return -1;
        }
    #ifdef __DEBUG
        totalitem++;
    #endif // __DEBUG
        strcpy(name,current->mtitle,maxleng);

        current->enable=true;
        current->showed=false;
        current->box.y=barbox.y+thick;
        current->box.h=barbox.h-thick*2;

        if (firstmenu==NULL) {
            current->box.x=thick;
            current->box.w=gettxtwith(current->mtitle);
            firstmenu=current;              //if is first menu
        } else  {
            ptr=getlastmenu(index);             //get item on barmenu or lastitem if not found
            if (ptr->item==index) {
                current->left=ptr->left;
                if (ptr->left!=NULL)
                    ptr->left->right=current;
                else
                    firstmenu=current;
                current->right=ptr;
                ptr->left=current;
                current->item=index;
                adjsmbox(current);
                while (ptr!=NULL) {
                    ptr->item++;
                    ptr=ptr->right;
                }
            } else {
            current->left=ptr;
            ptr->right=current;
            current->item=(ptr->item)+1;
            adjsmbox(ptr);
            //current->box.x=ptr->box.x+ptr->box.w+SPACE;
            }
        }
        return current->item;
};

/************************************************************************
 * Definicion del comportamiento: El menu de la barra superior es una   *
 *  lista horizontal, el primero elemento a la izquierda no tiene parent*
 *  de este cuelga a su derecha el segundo elemento este tiene como     *
 *  parent el de su izquierda y como right el de su derecha, de cada *
 *  uno de estos elementos podemos colgar en el down una lista      *
 *  vertical
 *  TODO: Pensar en un sistema distinto para colgar los item de la barra*
 *        separar el numero de item del tipo 0 de los item de otros tipos
 *  Utiliza calloc por lo que luego hay que liberar la memoria allocatada *
 *  Utiliza getlastitem para buscar el item donde colgar el item actual
 */
int32_t WMenu::addItem(int32_t parentitem, const char name[],int32_t item, int32_t type, SDL_Surface* sfc){
        int32_t litem=WMENU_ITEM_ANY;
        Menu_t* mptr=NULL;
        Menu_t* ptr=NULL;
        Menu_t* current=(Menu_t*)calloc(1,sizeof(Menu_t));
        int shiftX= (type>1)? itemheight:0;
    #ifdef __DEBUG
        totalitem++;
    #endif // __DEBUG

        litem=findFreeItem(item);
        if(litem!=WMENU_ITEM_ANY) {
            mptr=itemsearch(firstmenu,parentitem);
            if (mptr!=NULL && current!=NULL) {                   //Parent found, colgaremos current debajo del menu (set current as posible)
                //TODO: Reservar memoria aqui mejor que arriba
                memset(current,0,sizeof(Menu_t));
                strcpy(name,current->mtitle,maxleng);
                current->item=litem;

                // Hasta aqui pueden pasar 3 cosas:
                //  1º el item encontrado es un menubar ==>
                //      buscamos el ultimo colgado debajo y colgamos current
                //  2º el item encontrado es un submenu y ya tiene mas subitem colgados a su derecha ==>
                //      buscamos el ultimo de los item colgados a su derecha y colgamos current
                //  3º el item encontrado es un submenu y no tiene ningun item a su derecha ==>
                //      colgamos current a la dercha de mptr
                //mptr->smbox.y=mptr->box.y+mptr->box.h;

                if (mptr->type==0) {             //Si el item encontrado esta en la barra  lo colgamos debajo del ultimo
                    ptr=getlastitem(mptr);
                    current->up=ptr;
                    ptr->down=current;
                    current->box.y=ptr->box.y + ptr->box.h + ((ptr==mptr)? thick:0);     //Esto hay que mejorarlo
                    mptr->smbox.y=mptr->box.y+mptr->box.h;
                    mptr->smbox.h+=(mptr->smbox.h<thick)? thick*2:0;    //(ptr==mptr)? thick*2:0;
                } else if (mptr->right!=NULL) { //El item encontrado es un subitem con otros colgados a su derecha
                    ptr=getlastitem(mptr->right); //Lo colgaremos debajo del ultimo item que esté a su derecha
                    current->up=ptr;
                    ptr->down=current;
                    current->box.y=ptr->box.y+ptr->box.h;     //Esto hay que mejorarlo
                } else {                        //El item encontrado es un subitem sin otros item a su derecha
                    ptr=mptr;
                    current->left=mptr;         //Lo colgaremos el primero a su derecha
                    mptr->right=current;
                    current->box.x+=mptr->box.w-5;      //5 = solape
                    current->box.y=mptr->box.y;
                    mptr->smbox.y=mptr->box.y-thick;
                    mptr->smbox.h+=thick*2;
                }
                current->type=type | 1;
                current->enable=true;
                current->box.x+=ptr->box.x;
                current->box.h=itemheight;
                current->box.w = gettxtwith(current->mtitle) + gettxtwith("  >") + shiftX;//((type>1)? itemheight:0);
                current->shiftX=shiftX;
                if (type==WM_IMAGE && sfc!=NULL)
                    current->image=sfc;
                //current->item=item;
                //Si nuestro item será desplazado a la derecha nos aseguramos de los anteriores tambien lo estén
                //Si nuestro item no necesita estar desplazado a la derecha pero el anterior lo está entonces
                // el actual tambien ha de estarlo.
                Menu_t* lptr=current->up;
                if (type>1) {                               // Si nuestro item>1 => corrregimos todos los anteriores
                    while (lptr!=NULL && lptr->type>0) {
                        lptr->box.w += (lptr->shiftX==0)? shiftX:0;
                        lptr->shiftX = shiftX;
                        lptr=lptr->up;
                    }
                } else if (current->up!=NULL) {             // Si nuestro item=1 => copiamos shiftX del anterior si existe
                    current->box.w += current->up->shiftX;
                    current->shiftX=current->up->shiftX;
                }
                //Igual que lo anterior pero con mas iteraciones
                //NO BORRAR hasta verificar que el anterior no tiene problemas
    //            int shiftX=0;
    //            while (lptr->up!=NULL && lptr->type>0) {
    //                if (lptr->type > 1)
    //                    shiftX=itemheight;
    //                lptr=lptr->up;
    //            }
    //            while (lptr!=NULL) {
    //                lptr->shiftX=shiftX;
    //                lptr=lptr->down;
    //            }


                mptr->smbox.x=current->box.x-thick;
                mptr->smbox.h+=current->box.h;

                if (current->box.w > mptr->smbox.w) {  //current->smbox.w) {
                    mptr->smbox.w=current->box.w+thick*2;
                    ptr=current->up;
                    while(ptr!=NULL) {
                        if(ptr->type>0)
                            ptr->box.w=current->box.w;
                        ptr=ptr->up;
                    }
                } else current->box.w = mptr->smbox.w-thick*2;
                if (mptr->bkgsurface!=NULL)
                    SDL_FreeSurface(mptr->bkgsurface);
                mptr->bkgsurface=SDL_CreateRGBSurfaceWithFormat(0,mptr->smbox.w,mptr->smbox.h,32,SDL_PIXELFORMAT_ARGB8888);
                drawframe(mptr->bkgsurface,mcolor,lcolor,dcolor,thick,WMType);
            } else {
                if (mptr==NULL)
                    printf("Error... Parent menu item not found... Parent: %i\n\r",parentitem);
                else if(current==NULL)
                    printf("Error... No enouft memory to create new menu structure");
            }
        } else
            printf("Error... Nº of item xceded the limit, item > %i",((uint32_t)WMENU_ITEM_ANY)-1);
    return current->item;
};
/******************************************************************************
 * Get a item id currently free (unused) return item or -1 if not fount any
 * item free.
 * parameter:
 * item: item if >=0 is the item to iuse
 * return:
 * item passed if >=0 , new item id free, or -1 if not item free found
 ******************************************************************************/
int32_t WMenu::findFreeItem(int32_t item) {
    int32_t nrand=item;
    uint32_t count=0;

    if (item==WMENU_ITEM_ANY) {
        do {
            nrand = (rand()<<16) | ((rand()&0xFF)<<8) | (rand()&0xFF);
            count++;
        } while (itemsearch(firstmenu,nrand,1) !=NULL && count < (uint32_t)WMENU_ITEM_ANY);
        if (count & WMENU_ITEM_ANY)
            nrand=WMENU_ITEM_ANY;
    }
    return nrand;
}
/********************************************************************
 * Ajusta o reajusta el box donde se pintara cada uno de los item   *
 * de una instancia a partir del item apuntado por el parametro de  *
 * entrada ptr. Esto se hace cuando se le cambia el nombre a algun  *
 * item pudiendo hacer necesario reajustar todas las coordenadas de *
 * todos los subitem del menu desplegado y los siguientes menus de  *
 * la barra de menus. O por que se ha añadido o eliminado algun item*
 * Parametros:                                                      *
 *            ptr: Puntero al primer item a partir del cual se      *
 *                 reajustarán todas las coordenadas                *
 * Esta función no retorna nada.                                    *
 ********************************************************************/
void WMenu::adjsmbox(Menu_t* ptr) {
    if (ptr!=NULL) {
        int32_t shiftx=0;
        int32_t with=0;
        Menu_t* pptr=ptr;
        Menu_t* lptr=NULL;

        if (ptr->type==0) {
            ptr->box.w=gettxtwith(ptr->mtitle);
            if (ptr->left!=NULL) {
                ptr->box.x=ptr->left->box.x+ptr->left->box.w+SPACE;
                ptr->smbox.x=ptr->box.x-thick;
            } else {
                ptr->box.x=thick;
                ptr->smbox.x=0;
            }
            if (ptr->right!=NULL)
                adjsmbox(ptr->right);
        }

        while (ptr->up!=NULL)
            ptr=ptr->up;                          // We go to top menu (first item)

        if (ptr->type==0) {                       // Get parent of item ptr
            pptr=ptr;
            ptr=ptr->down;
        } else pptr=ptr->left;

        pptr->smbox.w=pptr->smbox.h=0;
        lptr=ptr;
        while (lptr!=NULL) {
            if (lptr->type > 1) shiftx=itemheight;
            with=gettxtwith(lptr->mtitle) + gettxtwith("  >");
            if (with > pptr->smbox.w)
                pptr->smbox.w=with;
            lptr=lptr->down;
        }

        pptr->smbox.y= (pptr->type==0)? pptr->box.y+pptr->box.h:pptr->box.y-thick;

        while (ptr!=NULL) {                     // And now go down reposicioning all items
            ptr->box.x=pptr->smbox.x+thick;
            ptr->box.w=pptr->smbox.w + shiftx;//gettxtwith(ptr->mtitle) + gettxtwith("  >") + shiftx;
            ptr->shiftX=shiftx;
            ptr->box.y = pptr->smbox.y + pptr->smbox.h + thick;
            pptr->smbox.h+=itemheight;
            if (ptr->right!=NULL)
                adjsmbox(ptr->right);
            ptr=ptr->down;
        }
        if (pptr->smbox.h>=itemheight) {
            pptr->smbox.w+=shiftx+thick*2;
            pptr->smbox.h+=thick*2;
        }
        if (pptr->bkgsurface!=NULL)
            SDL_FreeSurface(pptr->bkgsurface);
        pptr->bkgsurface=SDL_CreateRGBSurfaceWithFormat(0,pptr->smbox.w,pptr->smbox.h,32,SDL_PIXELFORMAT_ARGB8888);
        drawframe(pptr->bkgsurface,mcolor,lcolor,dcolor,thick,WMType);
    }
}

/*********************************************************************
 * Elimina y destruye el primer item terminal con el id especificado,
 *   si del item cuelga algun subitem entonces el item solicitado no
 *   será borrado.
 * Nota: El item a eliminar podria estar repetido, la función elimina
 *        el primero que encuentre, para eliminar el resto se puede
 *        llamar a la función repetidas veces hasta que retorne 0.
 * Parametros:
 * item el numero del item a eliminar y destruir
 * Retorna: 1 si el item fué eliminado y 0 si no fué encontrado ó
 *          tenia algun subitem colgando de él.
 *********************************************************************/
int32_t WMenu::rmEndItem(int32_t item) {
    int32_t pret=totalitem;
    Menu_t* ptr=itemsearch(firstmenu,item,1);
    Menu_t* lptr=NULL;
    if (ptr!=NULL) {
        if (ptr->right==NULL) {
            if (ptr->down!=NULL) {
                ptr->down->up=ptr->up;
                lptr=ptr->down;
            }
            if (ptr->up!=NULL) {
                ptr->up->down=ptr->down;    //Si tiene un item arriba debera apuntar al que esta debajo
                lptr = ptr->up;
            }
            if (ptr->left!=NULL) {
                ptr->left->right=ptr->down; //Si tiene un left=> es el primero y su left apuntara al de abajo
                if (ptr->down!=NULL) {
                    ptr->down->left=ptr->left;  //Si queda uno debajo su left a puntará a ptr->left
                    ptr->down->up=NULL;         //Si queda uno debajo=> este sera el primero y anlamos el de arriba
                }
                lptr=ptr->left;
            }
            debugnl("Removed itemId: %i, name: %s",ptr->item,ptr->mtitle);
            adjsmbox(lptr);
//            if (ptr->image!=NULL)
//                SDL_FreeSurface(ptr->image);
            if (ptr->bkgsurface!=NULL) {
                SDL_FreeSurface(ptr->bkgsurface);
                ptr->bkgsurface=NULL;
            }
            totalitem--;
            free(ptr);
        }
    }
    return pret-totalitem;
}

/******************************************************************************
 * Elimina y destruye un item y todos los subitems que cuelgue de él.
 * Parametros:
 *  item: numero del item que será eliminado junto a todos los subitems que
 *        cuelguen de el.
 * Retorna el numero de items eliminados en total 0 si no se encuentra el item
 ******************************************************************************/
int32_t WMenu::delItems(int32_t item) {
    int32_t pret = totalitem;
    Menu_t* ptr=itemsearch(firstmenu,item,1);
    if (ptr!=NULL) {
        if (ptr->right) {
            debugnl("Borrando itemId: %i, itemName: %s type: %i",ptr->right->item,ptr->right->mtitle,ptr->right->type);
            rDelAllItem(ptr->right);
        }
        adjsmbox(rmItem(ptr));
    } else {
        debugnl("No se borró el item: %i por que no se ha encontrado",item);
    }
    debugnl("Total Items borrados: %i restantes: %i",pret-totalitem,totalitem);
    return pret-totalitem;
}


/****************************************************************************
 * Elimina un menu de la barra de menus con el indice solicitado si existe
 * Parametros:
 * index= indice del menu en la barra de menus, el primer menu es el 0 y
 *        se incrementa el indice con cada menu a la derecha.
 * Retorna el numero de items eliminados incluyendo el menu solicitado.
 ****************************************************************************/
int32_t WMenu::delMenu(int32_t index) {
    int32_t pret = totalitem;
    Menu_t* ptr=itemsearch(firstmenu,index,0);
    Menu_t* lptr=ptr;
    if (ptr!=NULL && ptr->type==0) {
        rDelAllItem(ptr->down);
        if (ptr->left!=NULL) {
            ptr->left->right=ptr->right;
        } else
            firstmenu=ptr->right;
        if (ptr->right!=NULL) {
            ptr->right->left=ptr->left;
        }
        debugnl("Borrando menuIndex: %i, itemName: %s type: %i",ptr->item,ptr->mtitle,ptr->type);
        while(lptr->right!=NULL) {
            lptr->right->item--;
            lptr=lptr->right;
        }
        if(ptr->left!=NULL)
            adjsmbox(ptr->left);
        else if(ptr->right)
            adjsmbox(firstmenu);

//        if(ptr->image!=NULL)
//            SDL_FreeSurface(ptr->image);
        if(ptr->bkgsurface!=NULL) {
            SDL_FreeSurface(ptr->bkgsurface);
            ptr->bkgsurface=NULL;
        }

        totalitem--;
        free(ptr);
    }
    return pret-totalitem;
}
/****************************************************************************
 * Inserta un item encima del item indicado si este es encontrado
 * Parametros:
 * parentitem = item solicitado sobre el que se insertará el nuevo item
 * *name[] = nombre del item
 * item = numero que se asignará al nuevo item, si se usa la constante
 *        WMENU_ITEM_ANY entonces la función buscará un item al azar no
 *        utilizado que será el que se asigne al nuevo item.
 * type = (opcional) tipo de item a crear, se puede usar WM_DEFAULT, WM_TOGLE,
 *        WM_IMAGE, segun el tipo de item deseado, si no se especifica se usará
 *        el tipo WM_TOGLE.
 * sfc =  Si el tipo era WM_IMAGE se podrá especificar un surface que será la
 *        imagen que aparecera a la izquierda del nombre del item.
 *        Tenga en cuenta que el surface no será eliminado si se destruye
 *        el item posteriormente, usted pasa el puntero al surface y usté
 *        destruye el surface.
 ****************************************************************************/
int32_t WMenu::insItem(int32_t parentitem, const char name[],int32_t item, int32_t type, SDL_Surface* sfc) {
    int32_t litem= findFreeItem(item);
    Menu_t* ptr=itemsearch(firstmenu,parentitem);
    if (ptr!=NULL && ptr->type>0) {
        Menu_t* current=(Menu_t*)calloc(1,sizeof(Menu_t));
        memset(current,0,sizeof(Menu_t));
        strcpy(name,current->mtitle,MENU_MAX_LENG);
        current->down=ptr;
        current->type=type | 1;
        current->item=item;
        current->box.x=ptr->box.x;
        current->box.h=itemheight;
        current->enable=true;
        if (type==WM_IMAGE && sfc!=NULL)
            current->image=sfc;
        if (ptr->up!=NULL) {                            //Si el item tiene otro encima lo insertamos en medio
            current->up=ptr->up;
            ptr->up->down=current;
        } else {
            current->left=ptr->left;
            ptr->left->right=current;
        }
        ptr->up=current;
        ptr->left=NULL;
        totalitem++;
        litem=1;
        adjsmbox(current);
    }

    return litem;
}
/*******************************************************************
 * Establece el surface con una imagen que será pintada si el
 * item es de tipo WM_IMAGE si no lo es este surface será ignorado
 * Parametros:
 *  item:   el item en el que se establecerá el surface con la imagen
 *  sfc:    el surface que colocará en este item, si el item tenia
 *          un surface con anterioridad uste es responsable de
 *          eliminarlo. Si este parametro es NULL la imagen será
 *          eliminada y ya no aparecerá en el item.
 * Retorna un puntero al surface que habia antes de la llamada a
 *         esta función o NULL si no habia imagen previa o el item
 *         no se encuentra o no es de tipo WM_IMAGE
 *******************************************************************/
SDL_Surface* WMenu::setItemImage(int32_t item,SDL_Surface* sfc) {
    SDL_Surface* pret=NULL;
    Menu_t* ptr=itemsearch(firstmenu,item,WM_IMAGE);
    if (ptr!=NULL && ptr->type==WM_IMAGE) {
        pret=ptr->image;
        ptr->image=sfc;
    }
    return pret;
}
#ifdef __DEBUG
Menu_t* WMenu::getitem(int32_t item) {
    return itemsearch(firstmenu,item,WM_DEFAULT);
}
#endif // __DEBUG

/********************************************************************
 * Busca un item de forma horizontal desde el primer item de la     *
 * barra de menu hacia la derecha moviendose por todos los item de  *
 * la barra de menu hasta encontrar el item del parametro de entrada*
 * item.                                                            *
 * Parametro:                                                       *
 *            item: El item a buscar en la barra de menu            *
 * Retorna un puntero al item buscado si se encuentra y NULL si no  *
 * se encuentra                                                     *
 ********************************************************************/
Menu_t* WMenu::getlastmenu(int32_t item) {
    Menu_t* ptr=firstmenu;
    while(ptr!=NULL && ptr->right!=NULL && item!=ptr->item)
        ptr=ptr->right;
    return ptr;
}
/********************************************************************
 * Retorna el ultimo item buscando hacia abajo a partir del parametro
 * de entrada ptr.                                                  *
 * Parametro:                                                       *
 *            ptr: Puntero a un item a partir del que buscaremos el *
 *                 ultimo que se encuentre por debajo de este item  *
 * Retorna un puntero al ultimo item buscando siempre hacia abajo   *
 ********************************************************************/
Menu_t* WMenu::getlastitem(Menu_t* ptr) {
    while(ptr!=NULL && ptr->down!=NULL)
        ptr=ptr->down;
    return ptr;
}


/********************************************************************
 * Devuelve la altura en pixeles de la barra de menu.
 ********************************************************************/
int WMenu::getBarHeight() {
    return barbox.h;
}
/********************************************************************
 * Esta función realiza una busqueda por todos los item de una      *
 * instancia de WMenu a partir del item de entrada apuntado por ptr *
 * buscando el item de entrada cuyo campo type sea >= que el        *
 * parametro de entrada type, esto se usa como filtro de forma que  *
 * podamos buscar item en toda las lista de item o solo los que no  *
 * estan en la barra de menu.                                       *
 * Parametros:                                                      *
 *            ptr:  Puntero al item o menu a partir del cual se     *
 *                  realizará la busqueda.                          *
 *            item: Identificador del item que buscará la función   *
 *            type: Si type= 0 se buscará entre todos los item,     *
 *                  si type= 1 se buscará entre todos los item que  *
 *                  que no pertenezcan a la barra de menu ya sean   *
 *                  WM_DEFAULT, WM_TOGLE o WM_IMAGE                 *
 *                  si type= 2 se buscará entre los item WM_TOGLE y *
 *                  WM_IMAGE.                                       *
 *                  si type= 3 se buscará solo entre item WM_IMAGE  *
 * Retorna un puntero al item si se ha encontrado o NULL si no se ha*
 *  encontrado.                                                     *
 ********************************************************************/
Menu_t* WMenu::itemsearch(Menu_t* ptr, int32_t item, int32_t type) {
    Menu_t* found=NULL;
    if (ptr!=NULL) {
        if (ptr->item!=item || type>ptr->type) {
            if (ptr->right!=NULL && found==NULL) {
                found=itemsearch(ptr->right,item,type);
            }
            if (ptr->down!=NULL && found==NULL)
                found=itemsearch(ptr->down,item,type);
        } else found=ptr;
    }
    return found;
}

/****************************************************************
 * Establece el campo enable de un item este campo indica si el
 * item será pintado normalmente o sombreado y sin los subitem
 * que de él cuelguen.
 * Parametros:
 *  item: el item en el que se escribirá el estado enable/disable
 *  st:   el estado que obtendra el item tras esta llamada.
 ****************************************************************/
void WMenu::setAble(int32_t item, bool st) {
    Menu_t* ptr= itemsearch(firstmenu,item);
    ptr->enable=st;
}

/****************************************************************
 * Obtiene el campo enable de un item esto indica si el item es
 * visible o sombreado y si al mover el raton sobre el se
 * desplegaran los subitem que cuelguen de el.
 * Parametros: item el item del que devolverá el campo enable
 * Retorna:    -1 si el item no se encuentra ó 0/1 segun el campo
 *             enable del item.
 ****************************************************************/
int32_t WMenu::getAble(int32_t item) {
    int32_t rstate=0;
    Menu_t* ptr=itemsearch(firstmenu,item);
    if (ptr!=NULL)
        if (ptr->enable==true)
            rstate=1;
        else rstate=0;
    else
        rstate = -1;
return rstate;
}

/***************************************************************
 * Establece el valor de campo data de un item si el item es
 * de tipo TOGLE entonces aqui estará el valor del TOGLE
 ***************************************************************/
void WMenu::setData(int32_t item, int data) {
    Menu_t* ptr=itemsearch(firstmenu,item);
    if (ptr!=NULL)
        ptr->data=data;
}

/********************************************************************
 * Estable el nombre o titulo de un item.                           *
 * Parametros:                                                      *
 *             ntitle: El nuevo nombre o texto que tendrá el item   *
 *             item:   El item al que cambiaremos el titulo         *
 * Retorna el numero de caracteres que se han copiado en el campo   *
 * mtitle del item seleccionado, si el item no se encuentra retorna *
 * un 0 indicando que el item no se ha encontrado o no existe       *
 ********************************************************************/
//Change or set item name
int32_t WMenu::setItemTitle(const char* ntitle, int32_t item) {
    int32_t pret=0;
    Menu_t* ptr= itemsearch(firstmenu,item);
    if (ptr!=NULL) {
        pret=strcpy(ntitle,ptr->mtitle,MENU_MAX_LENG);
        adjsmbox(ptr);
    }
    return pret;
}
/********************************************************************
 * Establece una función que será llamada cuando se pique en el item
 * correspondiente.
 * Parametros: item el item que debe ser picado para ejecutar la
 *             función.
 *             calback es la dirección de la función que se ejecutará
 ********************************************************************/
void WMenu::setItemCallBAck(int32_t item, void (*calback)(Menu_t*)) {
    Menu_t* ptr=itemsearch(firstmenu,item);
    if (ptr!=NULL && ptr->type>0 && ptr->enable && ptr->right==NULL)
        ptr->calback=calback;
}
/********************************************************************
 * Guarda un item en la pila lifo de la instancia WMenu             *
 * Parametros:                                                      *
 *            ptr: Puntero al item que se guardará en la pila, solo *
 *                 guardamos el puntero.                            *
 * Esta función no retorna nada                                     *
 ********************************************************************/
void WMenu::push(Menu_t* ptr) {
    WM_Stack_t* lptr=new WM_Stack_t {NULL,NULL};
    lptr->lastItem=lastItem;
    lptr->StackItem=ptr;
    lastItem=lptr;
}
/********************************************************************
 * Retorna un item de la pila de item previamente guardado en ella  *
 * si queda alguno o NULL si no queda ninguno item en la pila lifo  *
 ********************************************************************/
Menu_t* WMenu::pop() {
    WM_Stack_t* Sptr=lastItem;
    Menu_t* Mptr=NULL;
    if (lastItem!=NULL) {
        Mptr=lastItem->StackItem;
        lastItem=lastItem->lastItem;
        delete(Sptr);
    }
    return Mptr;
}

/********************************************************************
 * Esta función devuelve el siguiente item al item de entrada       *
 * teniendo en cuenta los item que estan desplegados (mostrados)    *
 * y el orden correcto en el que se tienen que devolver.            *
 * Parametro:                                                       *
 *            ptr: Puntero al iten previo al que devolveremos       *
 * Retorna:   Puntero al siguiente item del puntero de entrada      *
 ********************************************************************/
Menu_t* WMenu::nextItem(Menu_t* ptr) {

    if (ptr->type==0) {

        if (ptr->enable && (ptr->sel||ptr->showed) && ptr->down!=NULL)
            push(ptr->right);
        else
            return ptr->right;

    } else {
        if (ptr->enable && (ptr->sel||ptr->showed) && ptr->right!=NULL)
            push(ptr->right);
    }
    if (ptr->down!=NULL)
        return ptr->down;
    else
        return pop();
}

/************************************************************************
 * drawdownmenu-> despliega (dibuja) un menu, si el menu cuelga de la   *
 * la barra de menus (type=0) despliega el menu colgado debajo y si es  *
 * de otro tipo (type>0) despliega el menu colgado a la derecha del     *
 * menu recibido en ptr. Lo primero que hace es copiar el background    *
 * correspondiente despues dibuja el marco del menu y despues escribe   *
 * todos sus items.                                                     *
 * Parametros: ptr = el puntero al menu a desplegar                     *
 ************************************************************************/
void WMenu::drawdownmenu(Menu_t* ptr) {
    if ((ptr->type==0 && ptr->down) || (ptr->type>0 && ptr->right)) {
        if (!ptr->drawed) {
            ptr->drawed=true;
        }
        if (ptr->enable)
            SDL_BlitSurface(ptr->bkgsurface,0,wSurface,&ptr->smbox);
        if (ptr->enable && ptr->type>0)
            ptr=ptr->right;
        else
            ptr=ptr->down;

        while (ptr!=NULL) {
            drawitem(ptr,0);
            ptr=ptr->down;
        }
    }
}

/***********************************************************************
 * Dibuja el texto de un item en las coordenadas especificadas dentro
 * del surface de la ventana donde esta el WMenu
 * Parametros: x, y coordenadas donde pintar el texto referenciadas a
 *             los pixeles de la ventana.
 *             txt: es el testo que se pintará.
 *             color: el color del texto a pintar.
 ***********************************************************************/
void WMenu::drawtextat(int x, int y, const char* txt, uint32_t color) {
    if (fontid!=NULL && wSurface!=NULL && *txt!=0) {
        SDL_Color sdlcolor=__INT_TO_SDLCOLOR(color);
        SDL_Surface* chrsurface=TTF_RenderText_Blended(fontid,txt,sdlcolor);
        SDL_Rect itembox={x,y,chrsurface->w,chrsurface->h};
        x+=chrsurface->w;                                                   //TODO: Para que añadir a x que se perdera
        SDL_BlitSurface(chrsurface,0,wSurface,&itembox);
        SDL_FreeSurface(chrsurface);
    }
}
/***************************************************************
 * Dibuja un tick en lugar especificado por box del color color
 * Parametros: *box:   el rectangulo donde se pintará el tick
 *              color: el color con el que se pintará el tick
 ***************************************************************/
void WMenu::drawCheck(SDL_Rect* box, uint32_t color) {
    if (fontid!=NULL && wSurface!=NULL) {
        SDL_Rect rect=*box;
        uint16_t chk[4]={0x0020,0x221A,0,0};
        SDL_Color sdlcolor=__INT_TO_SDLCOLOR(color);
        SDL_Surface* chrsurface=TTF_RenderUNICODE_Blended(fontid,chk,sdlcolor);
        SDL_BlitSurface(chrsurface,0,wSurface,&rect);
        SDL_FreeSurface(chrsurface);
    }
}
/********************************************************************
 * Esta funcion dibuja un item apuntado por el puntero de entrada   *
 * ptr, segun el color indicado por la variable de entrada sel, si  *
 * el item es de tipo WM_TOGLE ó WM_IMAGE dibujará previamente el   *
 * tick o la imagen correspondiente.                                *
 * Parametros:                                                      *
 *              ptr: El puntero al item que se va ha dibujar        *
 *              sel: Variable que selecciona el color con el que se *
 *                   dibuja el item mcolor, scolor o pcolor         *
 * Retorna: nada.
 ********************************************************************/
void WMenu::drawitem(Menu_t* ptr, int sel) {
    SDL_FillRect(wSurface,&ptr->box,(sel==0)? mcolor:(sel==1)? scolor:pcolor);
    if (ptr->type>WM_DEFAULT && ptr->right==NULL) {
        //SDL_Rect checkbox=ptr->box;
        //checkbox.w=itemheight-2;checkbox.x++;checkbox.y++;checkbox.h-=3;
        SDL_Rect checkbox={ptr->box.x+1,ptr->box.y+1,itemheight-2,ptr->box.h-3};
        if (ptr->type!=WM_IMAGE) {
            SDL_FillRect(wSurface,&checkbox,(sel==1)? scolor:pcolor);
            if (ptr->data)
                drawCheck(&ptr->box,tcolor);
        } else if (ptr->image!=NULL) {
            SDL_BlitScaled(ptr->image,0,wSurface,&checkbox);
        }
    }

//    if (ptr->type>0 && ptr->right==NULL) {
//        drawtextat(ptr->box.x+ptr->shiftX,ptr->box.y,ptr->mtitle,(ptr->enable)? tcolor:dcolor);
//    } else if (ptr->type>0) {
//        drawtextat(ptr->box.x+ptr->shiftX,ptr->box.y,ptr->mtitle,(ptr->enable)? tcolor:dcolor);
//    } else {
//        drawtextat(ptr->box.x,ptr->box.y,ptr->mtitle,(ptr->enable)? tcolor:dcolor);
//    }

    if (ptr->type==0) { // && ptr->right==NULL) {
        drawtextat(ptr->box.x,ptr->box.y,ptr->mtitle,(ptr->enable)? tcolor:dcolor);
//    } else if (ptr->type>0) {
//        drawtextat(ptr->box.x+ptr->shiftX,ptr->box.y,ptr->mtitle,(ptr->enable)? tcolor:dcolor);
    } else {
        drawtextat(ptr->box.x+ptr->shiftX,ptr->box.y,ptr->mtitle,(ptr->enable)? tcolor:dcolor);
    }

    if (ptr->enable && ptr->type>0 && ptr->right!=NULL)
        drawtextat(ptr->box.x+ptr->box.w-arrowwith, ptr->box.y,">",tcolor);
}

/********************************************************************
 * Esta función recibe un puntero a item y recorre todos los item   *
 * predecessores hacia arriba o hacia la izquierda hasta llegar al  *
 * menu de la barra activando el miembro sel de cada uno de los item*
 * recorridos y assignando barpiked a menu de la barra del cuelgan  *
 * Parametro:                                                       *
 *            ptr: Puntero al item desde el que partiremos activando*
 *                el miembro sel hasta llegar al menu del que cuelga*
 * Esta función no retorna ningun valor pero modifica los campos de *
 * las estructuras de los item que ha recorrido.                    *
 ********************************************************************/
void WMenu::setsel(Menu_t* ptr) {
    if (ptr->type>0)
        ptr->sel=true;
    while (ptr!=NULL && ptr->type>0) {
        if (ptr->up)
            ptr=ptr->up;
        else {
            ptr=ptr->left;
            ptr->sel=true;
        }
    }
    if (ptr->type==0) {
        ptr->sel=barpiked;
    }
}
/********************************************************************
 * Esta función dibuja cada item o subitem de un menú pasado como   *
 * parametro de entrada, con el color adecuado segun si está:       *
 * seleccionado, tiene el mouseover o nada de lo anterior, llamando *
 * a drawitem() para los item normales y a drawdownmenu() para      *
 * dibujar los subitem si del item de entrada cuelgan otros subitem.*
 * Parametro:
 *            ptr: Puntero al la structura Menu_t del item a partir *
 *                 del cual se dibujaran todos los item por debajo  *
 *                 de él, si este item está seleccionado dibujará   *
 *                 tambien los subitem que cuelguen a la derecha de *
 *                 él.
 ********************************************************************/
void WMenu::drawall(Menu_t* ptr) {
    Menu_t* lptr=NULL;
    while (ptr!=NULL) {
        if (ptr->sel) {
            drawitem(ptr,2);
            drawdownmenu(ptr);
            //drawitem(ptr,2);
            if (ptr->type>0 && ptr->right!=NULL)
                lptr=ptr;
        } else if (ptr->mover) {
            drawitem(ptr,1);
        } else
            drawitem(ptr,0);

        if (lptr!=NULL && ptr->down==NULL) {
            drawdownmenu(lptr);
            lptr=NULL;
        }
        ptr=nextItem(ptr);
    }
}

/********************************************************************
 * Esta funcion es llamada por drawallmenu para dibujar la barra    *
 * del menú, los menus que contenga y los item que deban ser        *
 * dibujados para lo cual llama a su vez a drawall() ó drawitem     *
 * según corresponda para tener en cuenta el orden de los item a    *
 * dibujar para que siempre queden los subitem encima de los item de*
 * los cuelgan.                                                     *
 * Esta función no tiene parametros de entrada y no devuelve nada   *
 ********************************************************************/
void WMenu::drawWMenu() {
    static Menu_t* backupptr=NULL;
    SDL_BlitSurface(barSurface,0,wSurface,&barbox);
    if (barpiked && selptr!=NULL && selptr!=backupptr) {
        clrLastMenu();
        setsel(selptr);
    } else
        drawall(firstmenu);

    if (selptr!=NULL && selptr!=backupptr) {
        drawall(firstmenu);
        backupptr=selptr;
    } else if (unselptr!=NULL) {
        drawitem(unselptr,(barpiked && unselptr->type==0)? 2:0);
        backupptr=NULL;
    }
    selptr=unselptr=NULL;


}
/********************************************************************
 * Esta función se llama desde cualquier instancia de WMenu pero    *
 * afecta a todas las instancias ya que recorre todas instancias    *
 * dibujando todos los menus dejando para el final la instancia que *
 * que tenga el foco del raton ya que ésta se debe dibujar encima de*
 * todas las demas, se llama desde la función fuera de la Clase     *
 * WMenu WMenu_Update(). Y llama a la funcion drawWMenu para dibujar*
 * para dibujar el menu y los item de cada instancia                *
 ********************************************************************/
void WMenu::drawallmenu() {
    WMenu* menuptr=firstInstancePtr;
    WMenu* menupiked=NULL;
    while (menuptr!=NULL) {
        if (!menuptr->barpiked)
            menuptr->drawWMenu();
        else
            menupiked=menuptr;
        menuptr=menuptr->nextInstancePtr;
    }
    if (menupiked!=NULL)
        menupiked->drawWMenu();

}

/********************************************************************
 * Recorre todos los item de cada menu de la instancia para limpiar *
 * (poner a false) los miebros sel, drawed, y mover de la estructura*
 * Menu_t de cada item. Esto se hace para evitar que se dibujen los *
 * item de algun menú previamente dibujado o desplegado.            *
 * Sin parametros de entrada ni de salida.                          *
 ********************************************************************/
void WMenu::clrLastMenu() {
    Menu_t* ptr=firstmenu;
    while (ptr!=NULL) {
        if (ptr->type==0) {
            if (ptr->sel) {
                ptr->sel=ptr->drawed=ptr->mover=false;
                ptr=ptr->down;
            } else ptr=ptr->right;
        } else {
            //debugnl("Item: %s, Sel: %i, drawed: %i, mover: %i",ptr->mtitle,ptr->sel,ptr->drawed,ptr->mover);
            if (ptr->sel) {
                ptr->sel=ptr->drawed=ptr->mover=false;
                ptr=ptr->right;
            } else ptr=ptr->down;
        }
    }
}

/********************************************************************
 * Esta función comprueba si el ratón está sobre algun menu o item  *
 * de los que estén desplegados o visibles devolviendo un puntero   *
 * apuntando al item sobre el que esté el raton e igualando a NULL  *
 * el puntero del item que anteriormente tuviera el foco del raton  *
 * Parameter:                                                       *
 *           x,y: coordenadas del la posición del raton.            *
 * No retorna nada pero cambia el estado del miembro mover,  de la  *
 * estructura Menu_t tambien cambia las variables privadas de la    *
 * clase WMenu selptr, unselptr y moverptr.                         *
 ********************************************************************/
void WMenu::chkmouseover(int32_t x, int32_t y) {
    Menu_t* ptr=firstmenu;
    SDL_Point mouse={x,y};
    //mouseovermenu=false;
    while (ptr!=NULL) {
        if (SDL_PointInRect(&mouse,&ptr->box)) {                            //Mouse is over and box menu
            if (!ptr->mover) {
                ptr->mover=true;
                selptr=ptr;
                mouseovermenu=true;
                debugnl("Item: %s get the focus.",ptr->mtitle);
            }
            moverptr=ptr;
        } else if (ptr->mover) {
            debugnl("Item: %s lost the focus.",ptr->mtitle);
            ptr->mover=false;
            moverptr=NULL;
            unselptr=ptr;
            //mouseovermenu=false;
        }
        ptr=nextItem(ptr);
    }
}

/********************************************************************
 * Esta función devuelve el numero identificador de la instancia con*
 * la que se ha llamado.                                            *
 ********************************************************************/
int WMenu::getInstance() {
    return instance;
}

/****************************************************************************************
 * Cuando se produce algún evento de tipo mouse down o mouse mover se ejecuta esta      *
 * llamando a la función correspondiente chkmousepiked o chkmouseover para cada una de  *
 * las instancias de menu creadas, esto permite detectar si el raton está sobre         *
 * cualquier item o menu y actuar en consecuencia o llamar a la función callback que    *
 * corresponda.
 * Esta función no la puede llamar el usuario ya que es un callback asignado cuando se  *
 * se crea la primera instancia de WMenu.
 * Parametros:
 *              m: puntero a la instancia WMenu de la que obtiene la 1ª instancia para  *
 *                 despues recorrer las demas instancias.
 *              event: El evento generado en el sistema, si el evento es mousedow o     *
 *                 mousemover llama a las funciones correspondientes y si el raton está *
 *                 sobre algún menu o item el evento sera filtrado y se insertará un    *
 *                 evento de typo usuario para que se analice en el programa principal ó*
 *                 si el item picado tiene un callback se llama a la función callback   *
 * Retorna: 1 si el evento ha de ser analizado por la aplicación o 0 si el evento es    *
 *          interceptado y anulado o cambiado por el de tipo usuario.                   *
 ****************************************************************************************/
int WMenu::myEventFilterFuntion(void* m,SDL_Event* event) {
int preturn=1;
if (WMenu::FirstInstance==NULL)
    return preturn;
WMenu* wmenu = reinterpret_cast <WMenu*> (m);
static bool menupickdown=false;
static bool delayedbarpiked=false;

Menu_t* lptr=NULL;


    if (event->type==SDL_MOUSEMOTION) {
        wmenu->chkmouseover(event->motion.x,event->motion.y);
        preturn = (wmenu->mouseovermenu)? 0:1;

    } else if (event->type==SDL_MOUSEBUTTONDOWN) {
        debugnl("\tMouse pikedDown with barpiked: %i",wmenu->barpiked);
        delayedbarpiked=wmenu->barpiked;
        lptr=wmenu->chkmousepiked();
        if (lptr!=NULL) {
            if (lptr->calback!=NULL && lptr->right==NULL)
                lptr->calback(lptr);
            if (lptr->type==WM_TOGLE)
                lptr->data^=1;
            wmenu->MenuEvent.user.code=lptr->item;
            wmenu->MenuEvent.user.data1=(void*)((intptr_t)wmenu->instance);
            wmenu->MenuEvent.user.data2=lptr;
            SDL_PushEvent(&wmenu->MenuEvent);
            debugnl("Piked on item %s instance: %i, with previous, menupickdown go to true",lptr->mtitle,wmenu->instance);
            menupickdown=true;
            preturn=0;
        } else {
                if (wmenu->barpiked) {
                    preturn=0;
                    menupickdown=true;
                    debugnl("Piked out menus with previous barpiked true, menupickdown go to true");
                } else if (delayedbarpiked) {
                    preturn=0;
                    menupickdown=true;
                    debugnl("Piked out menus with barpiked false & delayedbarpiked true nothing to do ");
                }
        }
    } else if (event->type==SDL_MOUSEBUTTONUP) {
        if (wmenu->barpiked  || menupickdown) {
            preturn= 0;
            debugnl("Release button with barpiked: %d, menupickdown now: %d go to true",wmenu->barpiked,menupickdown);
            menupickdown=false;
        }
    } else if (event->type==SDL_WINDOWEVENT) {
        if(event->window.event==SDL_WINDOWEVENT_SIZE_CHANGED) {
            wmenu->barbox.w=event->window.data1;
            SDL_FreeSurface(wmenu->barSurface);
            wmenu->barSurface=SDL_CreateRGBSurfaceWithFormat(0,wmenu->barbox.w,wmenu->barbox.h,32,SDL_PIXELFORMAT_ABGR8888);
            wmenu->drawframe(wmenu->barSurface,wmenu->mcolor,wmenu->lcolor,wmenu->dcolor,wmenu->thick,wmenu->WMType);
        }
    }

    if (wmenu!=NULL && wmenu->nextInstancePtr!=NULL) {
        preturn&=myEventFilterFuntion(wmenu->nextInstancePtr,event);
    }

    if (preturn && event->type != wmenu->myEventType)
        event->button.y-=wmenu->barbox.h;

return preturn;

}

/*******************************************************************
 * Chequea si se ha picado en algun item de la lista de menus, para
 * lo cual previamente ha de llamarse a chkmouseover() que prepara
 * un puntero (moverptr) al item sobre el cual esta posicionado
 * el puntero del raton antes de picar sobre el boton izquierdo,
 * gestiona la variable barpiked que indica si la barra de menu está
 * picada y por tanto los menus han de desplegarse al mover el raton
 * sobre ellos. Si el puntero al picarse no esta sobre algun item
 * ó se pica sobre algun item entonces limpia todos los item que
 * estuvieran desplegados y cambia el estado de la variable barpiked
 * a false.
 * TODO: continuar explicando esta función en detalle
 *******************************************************************/
Menu_t* WMenu::chkmousepiked() {
    Menu_t* lptr=NULL;
    if (moverptr!=NULL) {           //Si el puntero esta sobre un item al picar el boton derecho del raton
        barpiked=!barpiked;
        debugnl("Piked item %s id: %i",moverptr->mtitle,moverptr->item);
        if (barpiked) {             //Cuando pico en un item de la barra del menu
            moverptr->sel=true;
        } else {                    //Cuando una vez desplegado el menu pico en algun item del submenu o barra de menu
            if (moverptr->type>0) {
                barpiked=true;
                if (moverptr->right==NULL) {
                    barpiked=false;
                    clrLastMenu();
                    lptr=moverptr;
                }
            } else {
                barpiked=false;         //Parece irrelevante
                clrLastMenu();
            }
        }

        if (moverptr->type==0) {
            moverptr->mover=true;
            debugnl("Piked on %s",moverptr->mtitle);
        }
    } else {                    //Cuando pico fuera del menu limpio todos los menu y disable barpiked
        debugnl("Instance: %i Se ha picado fuera de todos los item del menu de esta instancia,\n\r\t clr all menu...",instance);
        clrLastMenu();
        barpiked=false;
    }
    if (moverptr!=NULL && moverptr->type>0 && !barpiked)
        moverptr=NULL;
    return lptr;
}

//static int WMenu::getInstances() {
//    return instances;
//}


/***************************************************************************
 * Destruye una variable de tipo WMenu, para ello primero destruye todos los
 * item que contenga y la memoria que tuvieran reservada, tras eso liberra
 * todos los recursos que tenga reservados el WMenu ajusta la lista de
 * instancias si hay mas de una variable tipo WMenu para mantenerlas
 * correctamente enlazadas. Decrementa el numero de instancias que haya pero
 * no cambia el identificador de las demas instancias para mantener la
 * coherencia con el programa.
 ***************************************************************************/
WMenu::~WMenu() {
    //Free menu item
    debugnl("Instance: %i Total item: %i",instance,totalitem);
    rDelAllItem(firstmenu);
    debugnl("Instance: %i Remaining items: %i",instance,totalitem);
    if (barSurface!=NULL) {
        SDL_FreeSurface(barSurface);
        barSurface=NULL;
    }
    if (fontid) {
        TTF_CloseFont(fontid);
        TTF_Quit();
    }
    firstmenu=NULL;
    fontid=NULL;
    moverptr=NULL;
    selptr=NULL;
    unselptr=NULL;

    if (lastInstancePtr!=NULL)
        lastInstancePtr->nextInstancePtr=nextInstancePtr;
    if (nextInstancePtr!=NULL)
        nextInstancePtr->lastInstancePtr=lastInstancePtr;
    if (this==firstInstancePtr){            //Setting this instance as first instance
        WMenu* wmptr=this->nextInstancePtr;
        while(wmptr!=NULL) {
            wmptr->firstInstancePtr=this->nextInstancePtr;
            wmptr=wmptr->nextInstancePtr;
        }
        FirstInstance=this->nextInstancePtr;
        if (FirstInstance!=NULL) {
            //SDL_SetEventFilter(this->nextInstancePtr->myEventFilterFuntion,this->nextInstancePtr);
            SDL_SetEventFilter(FirstInstance->myEventFilterFuntion,FirstInstance);
        }
    }
    firstInstancePtr=NULL;
    nextInstancePtr=NULL;
    lastInstancePtr=NULL;
    instances--;
}
/*******************************************************************************
 * Elimina un item terminal de la lista de menus esto es que no tenga ningun
 *  item a su derecha. Reajusta los punteros right up y down de sus item de
 *  alrededor para mantener la lista de menus correctamente enlazada.
 * Parameter: ptr es el item que sera eliminado si es un item terminal y no NULL
 * Retorna: puntero al que ocupa su lugar o su left si es el unico que hay.
 *******************************************************************************/
Menu_t* WMenu::rmItem(Menu_t* ptr) {
    Menu_t* pptr=ptr;
    if (ptr!=NULL) {
        if (ptr->right==NULL) {             //Solo si el item es un item terminal será borrado
            if (ptr->down!=NULL) {          //Si el item tiene otro debajo el de arriba apuntara al de abajo
                ptr->down->up=ptr->up;
                pptr=ptr->down;             //El item de abajo pasa a ser el parent item
            }
            if (ptr->up!=NULL) {
                ptr->up->down=ptr->down;    //Si tiene un item arriba debera apuntar al que esta debajo
                pptr = ptr->up;             // y el de arriba pasará a ser el parent item
            }
            if (ptr->left!=NULL) {
                ptr->left->right=ptr->down; //Si tiene un left=> es el primero y su left apuntara al de abajo
                pptr=ptr->left;             //Si el item y otro a la derecha este pasa a ser el parent
                if (ptr->down!=NULL) {
                    ptr->down->left=ptr->left;  //Si queda uno debajo su left a puntará a ptr->left
                    //ptr->down->up=NULL;         //Si queda uno debajo=> este sera el primero y anulamos el de arriba
                }
            }
            debugnl("Removed item name: %s, type: %i, id: %i",ptr->mtitle,ptr->type,ptr->item);
//            if (ptr->image!=NULL)
//                SDL_FreeSurface(ptr->image);
            if (ptr->bkgsurface!=NULL) {
                SDL_FreeSurface(ptr->bkgsurface);
                ptr->bkgsurface=NULL;
            }
            totalitem--;
            free(ptr);

        }
    }
    return pptr;
}

/***************************************************************************
 * Recorre toda la lista de menus llegando al ultimo item terminal y despues
 * llama a rmItem(ptr) para destruir el item sin romper la lista de menus
 * Parametros: ptr es el item inicial a partir del cual empezará a recorrer
 *             la lista de menus.
 * Retorna: puntero al parent del item eliminado o NULL si ptr era NULL
 ***************************************************************************/
Menu_t* WMenu::rDelAllItem(Menu_t* ptr) {
    if (ptr!=NULL) {
        if (ptr->right!=NULL) {
            ptr=rDelAllItem(ptr->right);
        }
        if (ptr->down!=NULL) {
            ptr=rDelAllItem(ptr->down);
        }
        ptr=rmItem(ptr);
    }
    return ptr;
}


/*******************************************************************************
 * Copia una cadena de caracteres desde src a dst hasta un maximo de maxcount
 * si tras copiar maxcount caracteres aun quedaban caracteres en src[] entonces
 * coloca ".." en los dos ultimos caracteres copiados.
 * Parameter:
 *  src[] : array de caracteres fuente de donde se copiará la cadena
 *  dst[] : array donde se copiará la cadena de caracteres se presupone que al
 *          menos dispone de maxcount caracteres
 *  maxcount: maximo numero de caracteres a copiar si la cadena src[] es mas
 *          larga que maxcount entonces los 2 ultimos caracteres en dst[] seran
 *          ".."
 * Retorna: el numero de caracteres copiados.
 *******************************************************************************/
uint32_t WMenu::strcpy(const char src[], char dst[], uint32_t maxcount){
    uint32_t index=0;
    while (src[index]!=0 && index<maxcount) {
        dst[index]=src[index];
            index++;
    }
    dst[index]=0;
    if(index>= maxcount && src[index]!=0) {
        dst[index-2]='.';
        dst[index-1]='.';
    }

    return index;
}

int WMenu::instances=0;
WMenu* WMenu::FirstInstance=NULL;

void WMenu_Update() {
    if (WMenu::FirstInstance!=NULL)
        WMenu::FirstInstance->drawallmenu();
}

//int32_t WMenu_EventInstance(SDL_Event &ev) {
//    return *((int32_t*)(&ev.user.data1));
//}
