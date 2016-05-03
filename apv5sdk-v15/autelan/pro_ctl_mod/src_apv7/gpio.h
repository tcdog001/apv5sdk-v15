extern static int mv_gpio_direction_input(struct gpio_chip *chip, unsigned pin);
extern static int mv_gpio_get_value(struct gpio_chip *chip, unsigned pin);
extern  static int mv_gpio_direction_output(struct gpio_chip *chip, unsigned pin,
	int value);
extern static void mv_gpio_set_value(struct gpio_chip *chip, unsigned pin,
	int value);
