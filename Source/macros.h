/*
 * IFJ 2017 project
 *	File			macros.h
 *	Description		Help for debugging, etc.
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		23:45, 09-10-2017
 */

#ifndef MACROS
#define MACROS

/* Přepínače */
#define DEBUG_ON

/* Definice */
#ifdef DEBUG_ON
#define $(...); puts("DEBUG: " #__VA_ARGS__);
#define $$(...); printf(__VA_ARGS__);
#else
#define $(...);
#define $$(...);
#endif

#endif