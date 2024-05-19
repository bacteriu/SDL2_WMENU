 /******************************************************************************************************
 * Versión: V0.027
 ******************************************************************************************************/

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cmath>
using namespace std;

#ifdef __linux__
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_ttf.h>
	#include <wiringPi.h>
#elif _WIN32 | _WIN64
    #if _WIN32
        #define IS32BIT
    #else
        #define IS64BIT
    #endif
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
	#define millis() 15
	#define uint uint16_t
#else
#endif


//#define __DEBUG         //Comentar esta definicion para eliminar los mensages de depuración
#ifdef __DEBUG
    #define debug(...) printf("%03i.%03i: ",SDL_GetTicks()/1000,SDL_GetTicks()%1000);printf(__VA_ARGS__)
    #define debugnl(...) printf("%03i.%03i: ",SDL_GetTicks()/1000,SDL_GetTicks()%1000);printf(__VA_ARGS__);printf("\n\r");
#else
    #define debugnl(...)
#endif // __DEBUG

#define __INT_TO_SDLCOLOR(X) {(uint8_t)(X>>16),(uint8_t)(X>>8),(uint8_t)(X),(uint8_t)(X>>24)}

#ifndef WMENU_H
#define WMENU_H

#define WMenu_EventInstance(x) *((int32_t*)(&(x).user.data1))

const int MENU_MAX_LENG=64;
const int32_t WMENU_ITEM_ANY=(int32_t)0x80000000;

enum Menu_Type {
    WM_DEFAULT  =1,
    WM_TOGLE    =3,
    WM_IMAGE    =5,
};

struct Menu_t {
    int32_t         type;
    int32_t         item;
    SDL_Rect        box;
    char            mtitle[MENU_MAX_LENG+1];
    bool            enable;
    bool            sel;
    bool            showed;
    bool            drawed;
    bool            mover;
    void            (*calback)(Menu_t*);
    int             data;
    Menu_t*         up;
    Menu_t*         down;
    Menu_t*         right;
    Menu_t*         left;
    SDL_Surface*    bkgsurface;
    SDL_Surface*    image;
    SDL_Rect        smbox;
    int32_t         shiftX;
};

//struct WMEventExInfo_t {
//    int32_t instance;
//    char mtitle[MENU_MAX_LENG];
//    Menu_t* menu;
//};

class WMenu {
public:
    static int32_t instances;
    static WMenu* FirstInstance;

    WMenu(SDL_Window* wID,const char* FName,uint32_t h,uint32_t tc, uint32_t mc, uint32_t lc, uint32_t dc, uint32_t type=0);
    int32_t addMenu(const char name[]);
    int32_t addItem(int32_t parentitem, const char name[],int32_t item, int32_t type=1,SDL_Surface* sfc=NULL);
    int32_t insItem(int32_t parentitem, const char name[],int32_t item, int32_t type=1, SDL_Surface* sfc=NULL);
    int32_t insMenu(int32_t index, const char name[]);
    int32_t rmEndItem(int32_t item);
    int32_t delItems(int32_t item);
    int32_t delMenu(int32_t index);
    SDL_Surface* setItemImage(int32_t item,SDL_Surface* sfc);
    //Menu_t* getitem(int32_t item);
    int32_t getBarHeight();
    bool setAble(int32_t item, bool st);
    int32_t getAble(int32_t item);
    bool setData(int32_t item, int data);
    bool getData(int32_t item, int &data);
    int32_t setItemTitle(const char* ntitle, int32_t item);
    int32_t getItemTitle(int32_t item, char* buftitle, int32_t leng);
    int32_t getItemType(int32_t item);
    void setItemCallBAck(int32_t item, void (* calback)(Menu_t*));
#ifdef __DEBUG
    Menu_t* getitem(int32_t item);
#endif
    int getInstance();
    void drawMenu();
    //static int getInstances();
    ~WMenu();
    int32_t inst;
protected:

private:
    int                 instance=0;
    struct WM_Stack_t {
        Menu_t*         StackItem=NULL;
        WM_Stack_t*     lastItem=NULL;
    };
    //Private functions
    void adjsmbox(Menu_t* ptr);
    Menu_t* getlastmenu(int32_t item=-1);
    Menu_t* getlastitem(Menu_t* ptr);
    Menu_t* itemsearch(Menu_t* ptr, int32_t item, int32_t type=0) ;
    Menu_t* rmItem(Menu_t* ptr);
    Menu_t* rDelAllItem(Menu_t* ptr);
    int gettxtwith(const char* txt);
    uint32_t strcpy(const char src[], char dst[], uint32_t maxcount);
    void push(Menu_t* ptr);
    Menu_t* pop();
    Menu_t* nextItem(Menu_t* ptr);
    Menu_t* chkmousepiked();
    int32_t findFreeItem(int32_t item);
    void chkmouseover(int32_t x, int32_t y);
    void drawdownmenu(Menu_t* ptr);
    void drawWMenu();
    void drawtextat(int x, int y, const char* txt, uint32_t color);
    void drawCheck(SDL_Rect* box, uint32_t color);
    void drawitem(Menu_t* ptr, int sel);
    void setsel(Menu_t* ptr);
    void drawall(Menu_t* ptr);
    void clrLastMenu();
    void drawframe(SDL_Surface* surface, /*SDL_Rect* box,*/ uint32_t mc, uint32_t lc, uint32_t dc, uint32_t tk, int type=0);
    static int myEventFilterFuntion(void* m,SDL_Event* event);

    // Var and structure definition
    const uint32_t      maxleng=MENU_MAX_LENG;
    const uint32_t      SPACE=30;
    int                 arrowwith=0;
    int32_t             itemheight=0;

    Menu_t*             firstmenu=NULL;
    Menu_t*             moverptr=NULL;
    Menu_t*             selptr=NULL;
    Menu_t*             unselptr=NULL;

    uint32_t            tcolor;        //Text color
    uint32_t            mcolor;        //Bacground Menu color
    uint32_t            lcolor;        //Light frame color
    uint32_t            dcolor;        //Dark frame color
    uint32_t            scolor;        //Item selected color
    uint32_t            pcolor;        //Item piked color
    uint32_t            WMType;
    int32_t             thick=4;

    TTF_Font*           fontid=NULL;
    int                 fontstyle;
    SDL_Rect            mbox={0};
    SDL_Window*         windowID=NULL;
    SDL_Rect            barbox={0};
    SDL_Surface*        wSurface=NULL;
    SDL_Surface*        barSurface=NULL;
    bool                barpiked=false;
    bool                mouseovermenu=false;

    WM_Stack_t*         lastItem=NULL;

//    int                 instance=0;

    SDL_Event           MenuEvent={0};
    Uint32              myEventType=(Uint32)-1;
    WMenu*              firstInstancePtr=NULL;
    WMenu*              nextInstancePtr=NULL;           //Creamos una lista de instancias WMenu Class
    WMenu*              lastInstancePtr=NULL;           // para movernos bidireccionalmente por las instancias

    void*               otherInstancePtr=NULL;          //Unimplemented
    SDL_EventFilter     otherEventFilter=0;

    int                 totalitem=0;
};
void WMenu_Update();
//int32_t WMenu_EventInstance(SDL_Event &ev);
#endif // WMENU_H
