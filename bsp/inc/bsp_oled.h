#ifndef BSP_OLED_H_
#define BSP_OLED_H_

extern int oled_init(int dc_pin, int res_pin);
extern void oled_deinit(int dev_fd);
extern int oled_disp_8x16char(int dev_fd, int x, int y, char ch);
extern int oled_disp_8x16str(int dev_fd, int x, int y, char ch[]);
extern int oled_clean_screen(int dev_fd);


#define OLED_WIDTH              (132)
#define OLED_HEIGHT             (64)

#endif
