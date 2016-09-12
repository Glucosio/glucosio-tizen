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

/**
 * Update all arrows to given time.
 */
void face_time_ui_update(int hour24, int minute, int second);

#endif /* FACE_H_ */
