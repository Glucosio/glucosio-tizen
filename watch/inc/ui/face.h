#ifndef FACE_H_
#define FACE_H_

/**
 * Create watch ui with given width and height
 */
void face_create(int width, int height);

/**
 * Destroy all UI by deleting main window and its child widgets.
 */
void face_destroy();

/*
 * Show indicator if it is not shown yet.
 * Update measurement value if it is shown.
 */
void face_glucose_indicator_show();

/*
 * Hide indicator if it is shown.
 */
void face_glucose_indicator_hide();

/**
 * Update all arrows to given time.
 */
void face_time_ui_update(int hour24, int minute, int second);

#endif /* FACE_H_ */
