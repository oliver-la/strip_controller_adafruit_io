RGBW current_value = { 0, 0, 0, 0 };

bool check_if_rgbw_identical(RGBW a, RGBW b) {
  return (
    a.r == b.r &&
    a.g == b.g &&
    a.b == b.b &&
    a.w == b.w
  );
}

bool check_if_rgbw_is_within_bounds(RGBW input) {
  return (input.r <= 255 && input.g <= 255 && input.b <= 255 && input.w <= 255);
}

RGBW apply_filter_brightness(RGBW input, byte brightness) {
  return (RGBW) {
    floor(float(input.r) / 100.0 * float(brightness)),
    floor(float(input.g) / 100.0 * float(brightness)),
    floor(float(input.b) / 100.0 * float(brightness)),
    floor(float(input.w) / 100.0 * float(brightness))
  };
}

void change_to_rgbw_target(RGBW target) {
  if (!check_if_rgbw_is_within_bounds(target)) {
    return;
  }

  analogWrite(pin.r, target.r);
  current_value.r = target.r;

  analogWrite(pin.g, target.g);
  current_value.g = target.g;

  analogWrite(pin.b, target.b);
  current_value.b = target.b;

  analogWrite(pin.w, target.w);
  current_value.w = target.w;
}

/**
 * Fades synchronously to the given color in 20ms increments.
 */
void apply_effect_fade(RGBW target, int fps = 20) {
  if (!check_if_rgbw_is_within_bounds(target)) {
    return;
  }

  RGBW intermediate_target = current_value;
  
  while (!check_if_rgbw_identical(current_value, target)) {
    // RED
    if (intermediate_target.r < target.r) {
      intermediate_target.r++;
    }
    if (intermediate_target.r > target.r) {
      intermediate_target.r--;
    }

    // GREEN
    if (intermediate_target.g < target.g) {
      intermediate_target.g++;
    }
    if (intermediate_target.g > target.g) {
      intermediate_target.g--;
    }

    // BLUE
    if (intermediate_target.b < target.b) {
      intermediate_target.b++;
    }
    if (intermediate_target.b > target.b) {
      intermediate_target.b--;
    }

    // WHITE
    if (intermediate_target.w < target.w) {
      intermediate_target.w++;
    }
    if (intermediate_target.w > target.w) {
      intermediate_target.w--;
    }

    change_to_rgbw_target(intermediate_target);
    delay(fps);
  }
}

/**
 * Fades synchronously to the given brightness in 20ms increments.
 */
void apply_effect_brightness_fade(byte brightness) {
  // If we just request a brightness change and there's nothing to do, we'll just default to white being dimmed.
  if (check_if_rgbw_identical(current_value, (RGBW){0,0,0,0}) && brightness > 0) {
    change_to_rgbw_target((RGBW){0,0,0,255});
  }
  apply_effect_fade(apply_filter_brightness(current_value, brightness));
}
