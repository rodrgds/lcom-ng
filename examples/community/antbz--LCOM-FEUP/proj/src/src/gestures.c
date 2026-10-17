#include "../headers/gestures.h"

enum mouse_state_t cutting_state = INIT;

void check_cutting(mouse_event event, double* slope, uint16_t* x_i, uint16_t* y_i, uint16_t* x_f, uint16_t* y_f) {
  switch (cutting_state)
  {
  case INIT:
    if (event.type == LB_PRESS) {
      cutting_state = START_CUT;
      //if (event.x_pos >= 0) {
        *x_i = event.x_pos;
      //} else {
      //  *x_i = 0;
      //}
      //if (event.y_pos >= 0) {
        *y_i = event.y_pos;
      //} else {
      //  *y_i = 0;
      //}
    } else {
      cutting_state = INIT;
    }
    break;
  case START_CUT:
    if (event.type == CUT_MOV) {
      cutting_state = CUTTING;
    } else {
      cutting_state = INIT;
    }
    break;
  case CUTTING:
    if (event.type == CUT_MOV) {
      cutting_state = CUTTING;
    } else if (event.type == LB_RELEASE) {
      cutting_state = COMP;
      //if (event.x_pos >= 0) {
        *x_f = event.x_pos;
      //} else {
      //  *x_f = 0;
      //}
      //if (event.y_pos >= 0) {
        *y_f = event.y_pos;
      //} else {
      //  *y_f = 0;
      //}
    } else {
      cutting_state = INIT;
    }
    break;
  case COMP:
    if (event.type == LB_PRESS) {
      cutting_state = START_CUT;
      //if (event.x_pos >= 0) {
        *x_i = event.x_pos;
      //} else {
      //  *x_i = 0;
      //}
      //if (event.y_pos >= 0) {
        *y_i = event.y_pos;
      //} else {
      //  *y_i = 0;
      //}
    } else {
      cutting_state = COMP;
    }
  default:
    break;
  }
}

mouse_event detect_mouse_event(struct packet* pp, Fruit* mouse_pointer, double* slope, uint16_t tolerance) {
  mouse_event ret;
  if (pp->lb && !pp->delta_x && !pp->delta_y) { // When left button is pressed - Start of cut
    ret.type = LB_PRESS;
    if (mouse_pointer->x_pos >= 0) {
      ret.x_pos = mouse_pointer->x_pos;
    } else {
      ret.x_pos = 0;
    }
    if (mouse_pointer->y_pos + mouse_pointer->chopped.height >= 0) {
      ret.y_pos = mouse_pointer->y_pos + mouse_pointer->chopped.height;
    } else {
      ret.y_pos = 0;
    }
    return ret;
  } else if (pp->lb && pp->delta_x && pp->delta_y && cutting_state == START_CUT) { // LB is pressed and mouse moves, gather data for slope info
    ret.type = CUT_MOV;
    *slope = (double)pp->delta_y / (double)pp->delta_x;
    return ret;
  } else if (pp->lb && !pp->delta_x && pp->delta_y && cutting_state == START_CUT) {
    ret.type = CUT_MOV;
    *slope = INFINITY;
    return ret;
  } else if (pp->lb && pp->delta_x && !pp->delta_y && cutting_state == START_CUT) {
    ret.type = CUT_MOV;
    *slope = 0.0;
    return ret;
  } else if (pp->lb && (pp->delta_x || pp->delta_y) && cutting_state == CUTTING) {
    if (pp->delta_x) {
      double nslope = (double)pp->delta_y / (double)pp->delta_x;
      if (abs(*slope - nslope) <= tolerance) {
        ret.type = CUT_MOV;
        return ret;
      } else {
        ret.type = DEFAULT_EV;
        return ret;
      }
    } else {
      if (*slope == INFINITY) {
        ret.type = CUT_MOV;
        return ret;
      } else if (pp->delta_y <= tolerance) {
        ret.type = CUT_MOV;
        return ret;
      } else {
        ret.type = DEFAULT_EV;
        return ret;
      }
    }
  } else if (cutting_state == CUTTING && !pp->lb) {
    ret.type = LB_RELEASE;
    if (mouse_pointer->x_pos >= 0) {
      ret.x_pos = mouse_pointer->x_pos;
    } else {
      ret.x_pos = 0;
    }
    if (mouse_pointer->y_pos + mouse_pointer->chopped.height >= 0) {
      ret.y_pos = mouse_pointer->y_pos + mouse_pointer->chopped.height;
    } else {
      ret.y_pos = 0;
    }
    return ret;
  } else {
    ret.type = DEFAULT_EV;
    return ret;
  }
}
