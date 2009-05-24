#ifndef SERVER_SELECT_H
#define SERVER_SELECT_H

extern void select_restart();
extern void select_add_sock_for_read(int sock);
extern void select_add_sock_for_write(int sock);
extern int select_action();
extern int select_is_change_sock(int sock);
extern int select_is_change_sock_for_read(int sock);
extern int select_is_change_sock_for_write(int sock);

#endif /* SERVER_SELECT_H */
