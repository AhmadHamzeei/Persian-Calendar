#include <gtk/gtk.h>
#include <jalali/jalali.h>

/* function to clear label color */
void clearcolor(GtkWidget *label,
                GtkWidget *eventbox)
{
  gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, NULL);
  gtk_widget_override_background_color(eventbox, GTK_STATE_FLAG_NORMAL, NULL);
}

/* function to highlight label */
void highlight(GtkWidget *eventbox,
               GtkWidget *window)
{
  GtkWidget *label, *lighted_eventbox, *lighted_label;
  GList *child;
  GtkStyleContext *context;
  GdkRGBA selected_fg;
  GdkRGBA selected_bg;
  
  /* get label from eventbox */
  child = gtk_container_get_children(GTK_CONTAINER(eventbox));
  label = child->data;
  
  /* prepare highlight colors */
  context = gtk_style_context_new();
  gtk_style_context_lookup_color(context, "selected_fg_color", &selected_fg);
  gtk_style_context_lookup_color(context, "selected_bg_color", &selected_bg);
  
  /* clearcolor previous highlighted label */
  lighted_eventbox = g_object_get_data(G_OBJECT(window), "lighted_eventbox");
  if(lighted_eventbox)
  {
    child = gtk_container_get_children(GTK_CONTAINER(lighted_eventbox));
    lighted_label = child->data;
    clearcolor(lighted_label, lighted_eventbox);
  }
  
  /* apply color */
  gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &selected_fg);
  gtk_widget_override_background_color(eventbox,
                                       GTK_STATE_FLAG_NORMAL, &selected_bg);
  
  /* set current eventbox as lighted */
  g_object_set_data(G_OBJECT(window), "lighted_eventbox", eventbox);
}

/* function to clear label */
void clearcal(GtkBuilder *builder)
{
  GtkWidget *label, *eventbox;
  gint i;
  gchar *setlabel, *seteventbox;
  
  /* we have 42 labels for showing month days, named label0-label41 */
  for(i = 0 ; i <= 41 ; i++)
  {
    /* find label & eventbox */
    setlabel = g_strdup_printf("label%d", i);
    label = GTK_WIDGET(gtk_builder_get_object(builder, setlabel));
    g_free(setlabel);
    seteventbox = g_strdup_printf("eventbox%d", i);
    eventbox = GTK_WIDGET(gtk_builder_get_object(builder, seteventbox));
    g_free(seteventbox);
    
    /* clear label color & text & day */
    clearcolor(label, eventbox);
    gtk_label_set_text(GTK_LABEL(label), NULL);
    g_object_set_data(G_OBJECT(eventbox), "day", NULL);
  }
}

/* function for converting digits to persian */
gchar *persian_digit(gint num)
{
  gchar *number = "";
  const gchar *dig[10] = {"۰", "۱", "۲", "۳", "۴", "۵", "۶", "۷", "۸", "۹"};
  gint i;
  
  do
  {
    i = num % 10;
    number = g_strconcat(dig[i], number, NULL);
  } while (num /= 10);
  
  return number;
}

/* function to get month name */
gchar *month_name(struct jtm today)
{
  gchar *name;
  gchar *mon[12] = {"فروردین", "اردیبهشت", "خرداد", "تیر", "مرداد", "شهریور",
                    "مهر", "آبان", "آذر", "دی", "بهمن", "اسفند"};
  
  return mon[today.tm_mon];
}

void showcal(GtkBuilder *builder)
{
  GtkWidget *label, *eventbox, *hbox, *window;
  gint i = 0, weeknum = 0;
  gchar *setlabel, *seteventbox, monthtxt[30], yeartxt[5], *day;
  
  /* get times from gtkbuilder */
  struct jtm *mytime = g_object_get_data(G_OBJECT(builder), "mytime");
  struct jtm *today  = g_object_get_data(G_OBJECT(builder), "today" );
  
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
  
  /* make a temporary time variable to use with while loop */
  struct jtm tmptime = *mytime;
  
  /* clear previous calendar */
  clearcal(builder);
  
  /* display new calendar */
  while(++i)
  {
    tmptime.tm_mday = i;
    jalali_update(&tmptime);
    
    /* break the loop at the end of the month */
    if(tmptime.tm_mday != i)
      break;
    
    /* count week number */
    if(tmptime.tm_wday == 0 && i != 1)
      weeknum++;
    
    /* find the correct label for setting the day number */
    setlabel = g_strdup_printf("label%d", weeknum * 7 + tmptime.tm_wday);
    label = GTK_WIDGET(gtk_builder_get_object(builder, setlabel));
    g_free(setlabel);
    
    /* find the correct eventbox for setting the highlight color */
    seteventbox = g_strdup_printf("eventbox%d", weeknum * 7 + tmptime.tm_wday);
    eventbox = GTK_WIDGET(gtk_builder_get_object(builder, seteventbox));
    g_free(seteventbox);
    
    /* set the day number */
    gtk_label_set_text(GTK_LABEL(label), persian_digit(i));
    
    /* set day to eventbox */
    day = g_strdup_printf("%d/%d", tmptime.tm_mon + 1, tmptime.tm_mday);
    g_object_set_data(G_OBJECT(eventbox), "day", day);
    
    /* highlight today */
    if(tmptime.tm_mday == today->tm_mday && 
       tmptime.tm_mon  == today->tm_mon  &&
       tmptime.tm_year == today->tm_year )
      highlight(eventbox, window);
  }
  
  /* hide the last hbox if we have 5 weeks */
  hbox = GTK_WIDGET(gtk_builder_get_object(builder, "box5"));
  if(weeknum + 1 == 5)
    gtk_widget_hide(hbox);
  else
    gtk_widget_show(hbox);
  
  /* set month label */
  label = GTK_WIDGET(gtk_builder_get_object(builder, "monthlabel"));
  jstrftime(monthtxt, 30, "%V", mytime);
  jstrftime(yeartxt, 5, "%Y", mytime);
  setlabel = g_strconcat(monthtxt, " ", persian_digit(atoi(yeartxt)), NULL);
  gtk_label_set_text(GTK_LABEL(label), setlabel);
  g_free(setlabel);
}

void on_next_button_click(GtkButton *button,
                          GtkBuilder *builder)
{
  struct jtm *mytime = g_object_get_data(G_OBJECT(builder), "mytime");
  
  /* increase month if it's not last month */
  if (mytime->tm_mon != 11)
    mytime->tm_mon ++;
  /* increase year if last month */
  else
  {
    mytime->tm_mon = 0;
    mytime->tm_year ++;
  }
  
  showcal(builder);
}

void on_pre_button_click(GtkButton *button,
                         GtkBuilder *builder)
{
  struct jtm *mytime = g_object_get_data(G_OBJECT(builder), "mytime");
  
  /* decrease month if it's not first month */
  if(mytime->tm_mon != 0)
    mytime->tm_mon --;
  /* decrease year if first month */
  else
  {
    mytime->tm_mon = 11;
    mytime->tm_year --;
  }
  
  showcal(builder);
}

gboolean on_eventbox_button_press_event(GtkWidget *eventbox, 
                                        GdkEventButton *event, 
                                        GtkWidget *window)
{
  /* get day status from eventbox */
  gchar *day = g_object_get_data(G_OBJECT(eventbox), "day");
  
  if (event->type == GDK_BUTTON_PRESS && day)
    highlight(eventbox, window);
  
  return FALSE;
}

void on_tray_icon_activate(GtkStatusIcon *status_icon,
                           GtkWindow *window)
{
  /* show main window & set focus */
  gtk_window_present(window);
}

void on_tray_icon_popup_menu(GtkStatusIcon *tray_icon,
                             guint button,
                             guint32 activate_time,
                             gpointer tray_menu)
{
    gtk_menu_popup(GTK_MENU(tray_menu), NULL, NULL,
                   gtk_status_icon_position_menu, tray_icon,
                   button, activate_time);
}

void text_request_callback(GtkClipboard *clipboard,
                           const gchar *text,
                           gpointer today_text)
{
  gtk_clipboard_set_text(clipboard, (gchar*)today_text, -1);
}

void on_menu_cal_click(GtkMenuItem *item,
                       gchar *today_text)
{
  GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);;
  gtk_clipboard_request_text(clipboard, text_request_callback, today_text);
}

/* functin to create tray icon and it's menu */
void create_tray_icon(GtkBuilder *builder)
{
  GtkWidget *tray_menu, *menu_cal, *menu_exit;
  GtkStatusIcon *tray_icon;
  gchar *path, *today_text;
  
  struct jtm *today = g_object_get_data(G_OBJECT(builder), "today");
  tray_icon = GTK_STATUS_ICON(gtk_builder_get_object(builder, "tray_icon"));
  tray_menu = GTK_WIDGET(gtk_builder_get_object(builder, "tray_menu"));
  
  path = g_strdup_printf("./pixmaps/ubuntu-mono-dark/persian-calendar-%d.png",
                         today->tm_mday);
  gtk_status_icon_set_from_file(tray_icon, path);
  
  today_text = g_strconcat(persian_digit(today->tm_mday), "", month_name(*today)
                           ,"", persian_digit(today->tm_year), NULL);
  menu_cal = gtk_menu_item_new_with_label(today_text);
  menu_exit = gtk_menu_item_new_with_label("خروج");
  g_signal_connect (menu_cal, "activate",
                      G_CALLBACK(on_menu_cal_click), (gpointer)today_text);
  g_signal_connect(menu_exit, "activate",
                   G_CALLBACK(gtk_main_quit), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(tray_menu), menu_cal);
  gtk_menu_shell_append(GTK_MENU_SHELL(tray_menu), menu_exit);
  
  gtk_widget_show_all(tray_menu);
}

int main(int argc,
         char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *pre_button, *next_button;
  struct jtm mytime, today;
  time_t now;
  
  gtk_init(&argc, &argv);
  
  /* set ui */
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "calendar.glade", NULL);
  
  /* initialize our time */
  now = time(NULL);
  jlocaltime_r(&now, &mytime);
  
  /* for storing today values, this should not change */
  today = mytime;
  
  /* attach our times to gtkbuilder */
  g_object_set_data(G_OBJECT(builder), "mytime", &mytime);
  g_object_set_data(G_OBJECT(builder), "today", &today);
  
  /* get ui elements from gtkbuilder */
  pre_button = GTK_WIDGET(gtk_builder_get_object(builder, "pre_button"));
  next_button = GTK_WIDGET(gtk_builder_get_object(builder, "next_button"));
  
  /* connect button actions */
  g_signal_connect(pre_button,  "clicked",
                   G_CALLBACK(on_pre_button_click),  (gpointer)builder);
  g_signal_connect(next_button, "clicked",
                   G_CALLBACK(on_next_button_click), (gpointer)builder);
  
  /* show initial calendar */
  showcal(builder);
  create_tray_icon(builder);
  
  gtk_builder_connect_signals(builder, NULL);
  
  gtk_main();
  
  return 0;
}
