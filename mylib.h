/*
 * mylib.h
 *
 *  Created on: 14.03.2018
 *      Author: Lo5mX
 */

#ifndef APP_TASK1_MYLIB_H_
#define APP_TASK1_MYLIB_H_

_Bool debounce(int port, const int pin);
void pen_up(void);
void pen_down(void);
void wait_for_end(OS_Q Q_STEP);
void diagonal(int times,_Bool xdir,_Bool ydir);
#endif /* APP_TASK1_MYLIB_H_ */
