#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "WMenu.h"
#ifdef __WIN32__ || __WIN64__
    #define fontpath "C:/Windows/Fonts/"
#else
    #define fontpath "Font/"
#endif // WIN32
using namespace std;

void exTogleItems(Menu_t* itemPtr){
    Menu_t* ptr=itemPtr;
    while(ptr->up!=NULL && ptr->up->type==WM_TOGLE) {
        ptr=ptr->up;
        ptr->data&=~1;
    }
    ptr=itemPtr;
    while(ptr!=NULL && ptr->type==WM_TOGLE) {
        ptr->data&=~1;
        ptr=ptr->down;
    }

}

int main( int argc, char* args[] )
{
    SDL_Surface* emoji[2]={NULL};
    int32_t data=0;
    //Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return 1;
	}

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return 2;
    }
    if ((emoji[0]=IMG_Load("Imagenes/EM_000.png")) == NULL) {
        printf("Error loading image emoji EM_000.png, Error: %s\n\r",IMG_GetError());
        return 3;
    } else
        SDL_SetColorKey(emoji[0], SDL_TRUE,0x0);

    if ((emoji[1]=IMG_Load("Imagenes/EM_001.png")) == NULL) {
        printf("Error loading image emoji EM_000.png, Error: %s\n\r",IMG_GetError());
        return 4;
    } else
        SDL_SetColorKey(emoji[1], SDL_TRUE,0x0);

    SDL_Window* hWindow = SDL_CreateWindow( "WMenu test by bacteriu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
    if( hWindow == NULL ) {
        printf( "Window could not be created! Error: %s\n", SDL_GetError() );
        return 5;
    }

    SDL_Surface* wSurface = SDL_GetWindowSurface( hWindow );

    SDL_Surface* surface= SDL_CreateRGBSurfaceWithFormat(0,wSurface->w,wSurface->h,32,SDL_PIXELFORMAT_RGB888);
    if (surface==NULL) {
        printf("Error no se pudo crear el surface principal, Error: %s\n\r",SDL_GetError());
        return 6;
    }

    WMenu menuOne(hWindow,fontpath"cambria.ttc",32,0x00000000,0xFFBDBDBD,0xFFFFFFFF,0XFF7B7B7B,0);
    menuOne.addMenu("Salir");
    menuOne.addMenu("Acciones");
    menuOne.addMenu("Help");
    menuOne.addItem(0,"Quit",10);
    menuOne.addItem(2,"About",20);

    int32_t emojiIndex=0,newMenu=0,newType=0,itemCount=0,newItems[10]={0};
    char buffer[64]={0};
    SDL_Surface* imgSurface=NULL;
    menuOne.addItem(1,"DEFAULT Item",11,WM_TOGLE);
    menuOne.addItem(1,"TOGLE Item",12,WM_TOGLE);
    menuOne.addItem(1,"Image Item",13,WM_TOGLE);
    menuOne.setData(11,1);
    menuOne.setItemCallBAck(11,exTogleItems);
    menuOne.setItemCallBAck(12,exTogleItems);
    menuOne.setItemCallBAck(13,exTogleItems);
    menuOne.addItem(1,"Add Menú",14);
    menuOne.addItem(1,"Insert Menñu",15);
    menuOne.addItem(1,"Add Item",16);
    menuOne.setAble(16,false);
    menuOne.addItem(1,"Insert Item",17);
    menuOne.setAble(17,false);

    menuOne.addItem(1,"Delete Menú",18);
    menuOne.setAble(18,false);
    menuOne.addItem(1,"Delete Item",19);
    menuOne.setAble(19,false);

    SDL_Rect box={0,0,surface->w,surface->h};
    SDL_FillRect(surface,&box,0x505050);


    cout << "WMenu Test by bacteriu" << endl;

    SDL_Event ev;
    bool quit=false;
    while (!quit) {
        while (SDL_PollEvent(&ev)!=0) {
            switch (ev.type) {
                case SDL_QUIT:
                    quit=true;
                break;
                case SDL_USEREVENT:
                    switch (ev.user.code) {
                        case 10:
                            quit=true;
                        break;
                        case 14:    //Añadir Menu
                        case 15:    //Insertar Menu
                            if (ev.user.code==14)
                                newMenu=menuOne.addMenu("Menu Añadido");
                            else
                                newMenu=menuOne.insMenu(2,"Menú Insertado");
                            menuOne.setAble(18,true);
                            menuOne.setAble(14,false);
                            menuOne.setAble(15,false);
                            menuOne.setAble(16,true);
                            menuOne.setAble(17,true);
                        break;
                        case 18:    //Eliminar Menu
                            menuOne.delMenu(newMenu);
                            menuOne.setAble(14,true);
                            menuOne.setAble(15,true);
                            menuOne.setAble(16,false);
                            menuOne.setAble(17,false);
                            menuOne.setAble(18,false);
                            menuOne.setAble(19,false);
                            itemCount=0;
                        break;
                        case 16:    //Añadir item
                        case 17:    //Insertar item
                            if (itemCount<10) {
                                menuOne.setAble(19,true);
                                int32_t data12;
                                int32_t data13;
                                if (menuOne.getData(12,data12) && data12) {
                                    newType=WM_TOGLE;
                                } else if (menuOne.getData(13,data13) && data13) {
                                    imgSurface=emoji[itemCount % 1];
                                    newType=WM_IMAGE;
                                } else {
                                    newType=WM_DEFAULT;
                                }
                                sprintf(buffer,"Item Añadido %i",itemCount);
                                if (ev.user.code==16)
                                    newItems[itemCount]=menuOne.addItem(newMenu,buffer,WMENU_ITEM_ANY,newType,imgSurface);
                                else
                                    newItems[itemCount]=menuOne.insItem(newMenu,buffer,WMENU_ITEM_ANY,newType,imgSurface);
                                itemCount++;
                                if (itemCount==10) {
                                    menuOne.setAble(16,false);
                                    menuOne.setAble(17,false);
                                }
                            }
                        break;
                        case 19:    //Delete Items
                            if (itemCount>0)
                                menuOne.delItems(newItems[--itemCount]);
                            if (itemCount==0)
                                menuOne.setAble(19,false);
                        break;
                        default:
                            if (ev.user.code > 19) {
                                //cout<< "Obtenidos "<< menuOne.getItemTitle(ev.user.code,buffer,63)<<" caracteres. ";
                                menuOne.getItemTitle(ev.user.code,buffer,63);
                                cout<<"Item: \""<< buffer <<"\", id: "<< ev.user.code << " piked";
                                if (menuOne.getItemType(ev.user.code)==WM_IMAGE) {
                                    emojiIndex = (emojiIndex + 1) % 2;
                                    menuOne.setItemImage(ev.user.code,emoji[emojiIndex]);
                                } else if (menuOne.getItemType(ev.user.code)==WM_TOGLE && menuOne.getData(ev.user.code,data)) {
                                    cout<<" state: ";
                                    (data&1)? cout<<"true":cout<<"false";
                                }
                                cout<<endl;
                            }
                    }
            }
        }

        SDL_BlitSurface(surface,0,wSurface,&box);
        WMenu_Update();
        SDL_UpdateWindowSurface(hWindow);
    }

    return 0;
}
