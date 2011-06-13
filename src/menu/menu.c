/*
 * This software is licensed under the terms of the MIT-License
 * See COPYING for further information. 
 * ---
 * Copyright (C) 2011, Lukas Weber <laochailan@web.de>
 */

#include "menu.h"
#include "global.h"

void add_menu_entry(MenuData *menu, char *name, void (*action)(void *), void *arg) {
	menu->entries = realloc(menu->entries, (++menu->ecount)*sizeof(MenuEntry));
	menu->entries[menu->ecount-1].name = malloc(strlen(name)+1);	
	strcpy(menu->entries[menu->ecount-1].name, name);
	menu->entries[menu->ecount-1].action = action;
	menu->entries[menu->ecount-1].arg = arg;
}

void destroy_menu(MenuData *menu) {
	int i;
	
	for(i = 0; i < menu->ecount; i++)
		free(menu->entries[i].name);
	
	free(menu->entries);
}

void create_menu(MenuData *menu) {
	memset(menu, 0, sizeof(MenuData));
	
	menu->fade = 1;
	menu->selected = -1;
}

void menu_input(MenuData *menu) {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		int sym = event.key.keysym.sym;
		if(event.type == SDL_KEYDOWN) {
			if(sym == tconfig.intval[KEY_DOWN] && menu->cursor < menu->ecount - 1) {
				menu->drawdata[3] = 10;
				menu->cursor++;
			} else if(sym == tconfig.intval[KEY_UP] && menu->cursor > 0) {
				menu->drawdata[3] = 10;
				menu->cursor--;
			} else if(sym == tconfig.intval[KEY_SHOT] || sym == SDLK_RETURN) {
				menu->quit = 1;
				menu->selected = menu->cursor;
			} else if(sym == SDLK_ESCAPE && menu->type == MT_Transient) {
				menu->quit = 1;
			}
			
		} else if(event.type == SDL_QUIT) {
			exit(1);
		}
	}
}

void menu_logic(MenuData *menu) {
	menu->frames++;
	if(menu->quit == 1 && menu->fade < 1.0)
		menu->fade += 1.0/FADE_TIME;
	if(menu->quit == 0 && menu->fade > 0)
		menu->fade -= 1.0/FADE_TIME;
	
	if(menu->fade < 0)
		menu->fade = 0;
	if(menu->fade > 1)
		menu->fade = 1;
		
	if(menu->quit == 1 && menu->fade >= 1.0) {
		menu->quit = menu->type == MT_Transient ? 2 : 0;
		if(menu->selected != -1) {
			menu->entries[menu->selected].action(menu->entries[menu->selected].arg);
// 			menu->selected = -1;
		}
	}
}

void fade_out(float f) {
	glColor4f(0,0,0,f);
			
	glBegin(GL_QUADS);
	glVertex3f(0,0,0);
	glVertex3f(0,SCREEN_H,0);
	glVertex3f(SCREEN_W,SCREEN_H,0);
	glVertex3f(SCREEN_W,0,0);
	glEnd();
	
	glColor4f(1,1,1,1);	
}