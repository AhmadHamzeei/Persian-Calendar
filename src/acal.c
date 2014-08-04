/*
 * acal.c - Simple persian calendar
 * 
 * Copyright 2014 Ahmad Hamzeei <ahmadhamzeei@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

#ifdef HAVE_JALALI_JALALI_H
#include <jalali/jalali.h>
#else
#include <jalali.h>
#endif

/* gtkbuilder & times global variables */
static GtkBuilder *builder;
struct jtm mytime, today;

/* day & month names */
gchar *day[] = {N_("Shanbeh"), N_("Yek-Shanbeh"), N_("Do-Shanbeh"),
                N_("Seh-Shanbeh"), N_("Chahaar-Shanbeh"),
                N_("Panj-Shanbeh"), N_("Jomeh")};
gchar *mon[] = {N_("Farvardin"), N_("Ordibehesht"), N_("Khordaad"), N_("Tir"),
                N_("Mordaad"), N_("Shahrivar"), N_("Mehr"), N_("Aabaan"),
                N_("Aazar"), N_("Dey"), N_("Bahman"), N_("Esfand")};

/* function to clear label color */
void clearcolor(GtkWidget *label,
                GtkWidget *eventbox)
{
  gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, NULL);
  gtk_widget_override_background_color(eventbox, GTK_STATE_FLAG_NORMAL, NULL);
}

/* function to highlight label */
void highlight(GtkWidget *eventbox)
{
  GtkWidget *label, *lighted_eventbox, *lighted_label;
  GList *child;
  GtkStyleContext *context;
  GdkRGBA selected_fg, selected_bg;
  
  /* get label from eventbox */
  child = gtk_container_get_children(GTK_CONTAINER(eventbox));
  label = child->data;
  
  /* prepare highlight colors */
  context = gtk_style_context_new();
  gtk_style_context_lookup_color(context, "selected_fg_color", &selected_fg);
  gtk_style_context_lookup_color(context, "selected_bg_color", &selected_bg);
  
  /* clearcolor previous highlighted label */
  lighted_eventbox = g_object_get_data(G_OBJECT(builder), "lighted_eventbox");
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
  g_object_set_data(G_OBJECT(builder), "lighted_eventbox", eventbox);
}

/* function to clear label */
void clearcal(void)
{
  GtkWidget *label, *eventbox;
  gchar *setlabel, *seteventbox;
  gint i;
  
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
  const gchar *dig[] = {N_("0"), N_("1"), N_("2"), N_("3"), N_("4"), N_("5"),
                        N_("6"), N_("7"), N_("8"), N_("9")};
  gint i;
  
  do
  {
    i = num % 10;
    number = g_strconcat(gettext(dig[i]), number, NULL);
  } while (num /= 10);
  
  return number;
}

void cal_gen(void)
{
  GtkWidget *label, *eventbox, *hbox;
  gint i = 0, weeknum = 0;
  gchar *setlabel, *seteventbox, *day;
  
  /* make a temporary time variable to use with while loop */
  struct jtm tmptime = mytime;
  
  /* clear previous calendar */
  clearcal();
  
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
    if(tmptime.tm_mday == today.tm_mday && 
       tmptime.tm_mon  == today.tm_mon  &&
       tmptime.tm_year == today.tm_year )
      highlight(eventbox);
  }
  
  /* hide the last hbox if we have 5 weeks */
  hbox = GTK_WIDGET(gtk_builder_get_object(builder, "box5"));
  if(weeknum + 1 == 5)
    gtk_widget_hide(hbox);
  else
    gtk_widget_show(hbox);
  
  /* set month label */
  label = GTK_WIDGET(gtk_builder_get_object(builder, "monthlabel"));
  setlabel = g_strdup_printf("%s %s", gettext(mon[mytime.tm_mon]),
                             persian_digit(mytime.tm_year));
  gtk_label_set_text(GTK_LABEL(label), setlabel);
  g_free(setlabel);
}

void on_next_month_click(GtkWidget *widget,
                         GdkEvent  *event,
                         gpointer *data)
{
  /* increase month if it's not last month */
  if (mytime.tm_mon != 11)
    mytime.tm_mon ++;
  /* increase year if last month */
  else
  {
    mytime.tm_mon = 0;
    mytime.tm_year ++;
  }
  
  cal_gen();
}

void on_pre_month_click(GtkWidget *widget,
                        GdkEvent  *event,
                        gpointer *data)
{
  /* decrease month if it's not first month */
  if(mytime.tm_mon != 0)
    mytime.tm_mon --;
  /* decrease year if first month */
  else
  {
    mytime.tm_mon = 11;
    mytime.tm_year --;
  }
  
  cal_gen();
}

gboolean on_eventbox_button_press_event(GtkWidget *eventbox, 
                                        GdkEventButton *event, 
                                        gpointer *data)
{
  /* get day status from eventbox */
  gchar *day = g_object_get_data(G_OBJECT(eventbox), "day");
  
  /* highlight if we have click event & day assigned to the eventbox */
  if (event->type == GDK_BUTTON_PRESS && day)
    highlight(eventbox);
  
  return TRUE;
}

void on_menu_app_click(GtkMenuItem *item,
                       GtkWindow *window)
{
  /* show main window & set focus */
  gtk_window_present(window);
}

void on_menu_cal_click(GtkAction *action,
                       gpointer data)
{
  const gchar *today_text;
  GtkClipboard *clipboard;
  
  today_text = gtk_action_get_label(action);
  /* tell the clipboard manager to make the data persistent */
  clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  gtk_clipboard_set_can_store(clipboard, NULL, 0);
  
  /* set clipboard text */
  gtk_clipboard_set_text(clipboard, today_text, -1);
}

void time_gen(void)
{
  time_t now;
  
  /* generate time */
  now = time(NULL);
  jlocaltime_r(&now, &today);
}

void update_tray(AppIndicator *indicator)
{
  GtkAction *action;
  gchar *icon_name, *today_text;
  
  action = GTK_ACTION(gtk_builder_get_object(builder, "menu_cal_action"));
  
  /* set tray icon */
  icon_name = g_strdup_printf("persian-calendar-%d", today.tm_mday);
  app_indicator_set_icon_full(indicator, icon_name, icon_name);
  g_free(icon_name);
  
  /* set menu_cal text */
  today_text = g_strdup_printf("%s %s %s %s", gettext(day[today.tm_wday]),
                               persian_digit(today.tm_mday),
                               gettext(mon[today.tm_mon]),
                               persian_digit(today.tm_year));
  gtk_action_set_label(action, today_text);
  g_free(today_text);
}

gboolean check_update_tray(gpointer indicator)
{
  /* save previous time values & generate new time */
  gint day = today.tm_mday, mon = today.tm_mon, year = today.tm_year;
  time_gen();
  
  /* update tray if day changes */
  if(today.tm_mday != day || today.tm_mon != mon || today.tm_year != year)
    update_tray(APP_INDICATOR(indicator));

  return G_SOURCE_CONTINUE;
}

void activate(GApplication *app,
              gpointer user_data)
{
  GtkAction *action;
  const gchar *today_text;
  GNotification *notification;
  
  action = GTK_ACTION(gtk_builder_get_object(builder, "menu_cal_action"));
  today_text = gtk_action_get_label(action);
  
  /* create a notify with today_text when app activates */
  notification = g_notification_new(_("Acal"));
  g_notification_set_body(notification, today_text);
  g_application_send_notification(app, "today-notify", notification);
  g_object_unref(notification);
}

void startup(GApplication *app,
             gpointer user_data)
{
  AppIndicator *indicator;
  GtkMenu *tray_menu;
  GtkAction *action;
  GtkWidget *menu_cal;
  GtkWindow *window;
  gchar *theme_path;
  
  /* set ui */
  builder = gtk_builder_new();
  gtk_builder_add_from_resource(builder, "/ui/calendar.glade", NULL);
  gtk_builder_connect_signals(builder, NULL);
  
  /* set gtkapplication window */
  window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
  gtk_application_add_window(GTK_APPLICATION(app), window);
  
  /* initialize our time */
  time_gen();
  mytime = today;
  
  /* create menu_cal item & add it to tray_menu */
  action = GTK_ACTION(gtk_builder_get_object(builder, "menu_cal_action"));
  tray_menu = GTK_MENU(gtk_builder_get_object(builder, "tray_menu"));
  menu_cal = gtk_action_create_menu_item(action);
  gtk_menu_shell_prepend(GTK_MENU_SHELL(tray_menu), menu_cal);
  
  /* prepare appindicator */
  theme_path = g_build_filename(DATADIR, "icons", "ubuntu-mono-dark",
                                "acal", NULL);
  indicator = app_indicator_new_with_path("Acal", "persian-calendar-1",
                                APP_INDICATOR_CATEGORY_APPLICATION_STATUS,
                                theme_path);
  g_free(theme_path);
  app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
  app_indicator_set_menu(indicator, tray_menu);
  
  /* show initial tray & prepare cal */
  update_tray(indicator);
  cal_gen();
  
  /* add timer to update tray */
  g_timeout_add_seconds(3, check_update_tray, (gpointer)indicator);
}

gint main(gint argc,
          gchar *argv[])
{
  GtkApplication *app;
  gint status;
  
  /* set up i18n */
  g_setenv("LANG", "fa_IR.UTF-8", TRUE);
  g_setenv("LANGUAGE", "fa.UTF-8", TRUE);
  bindtextdomain(PACKAGE, LOCALEDIR);
  bind_textdomain_codeset(PACKAGE, "UTF-8");
  textdomain(PACKAGE);
  
  /* creates a new gtkapplication instance */
  app = gtk_application_new("ir.ubuntu.acal", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  
  return status;
}
