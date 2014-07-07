#include <gtk/gtk.h>
#include <math.h>
#include <jalali/jalali.h>
#include <jalali/jtime.h>

/* function to clear label text */
void clearcal(GtkBuilder *builder)
{
  GtkWidget *label;
  gint i;
  gchar *setlabel;
  
  /* we have 42 labels for showing month days, named label0-label41 */
  for(i = 0 ; i <= 41 ; i++)
  {
    /* find the correct label name */
    setlabel = g_strdup_printf("label%d", i);
    
    /* clear label text */
    label = GTK_WIDGET(gtk_builder_get_object(builder, setlabel));
    g_free(setlabel);
    gtk_label_set_text(GTK_LABEL(label), NULL);
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

void showcal(GtkBuilder *builder)
{
  GtkWidget *label, *hbox, *monthlabel;
  gint i = 0, weeknum = 0;
  gchar *setlabel, monthtxt[30], yeartxt[5];
  GdkRGBA color;
  
  /* prepare blue highlight color */
  gdk_rgba_parse(&color, "#3465A4");
  
  /* get times from gtkbuilder */
  struct jtm *mytime = g_object_get_data(G_OBJECT(builder), "mytime");
  struct jtm *today  = g_object_get_data(G_OBJECT(builder), "today" );
  
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
    
    /* set the day number */
    label = GTK_WIDGET(gtk_builder_get_object(builder, setlabel));
    g_free(setlabel);
    gtk_label_set_text(GTK_LABEL(label), persian_digit(i));
    
    /* highlight today */
    if(tmptime.tm_mday == today->tm_mday && 
       tmptime.tm_mon  == today->tm_mon  &&
       tmptime.tm_year == today->tm_year )
      gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &color);
    else
      gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, NULL);
  }
  
  /* hide the last hbox if we have 5 weeks */
  hbox = GTK_WIDGET(gtk_builder_get_object(builder, "hbox"));
  if(weeknum + 1 == 5)
    gtk_widget_hide(hbox);
  else
    gtk_widget_show(hbox);
  
  /* set month label */
  monthlabel = GTK_WIDGET(gtk_builder_get_object(builder, "monthlabel"));
  jstrftime(monthtxt, 30, "%V", mytime);
  jstrftime(yeartxt, 5, "%Y", mytime);
  setlabel = g_strconcat(monthtxt, " ", persian_digit(atoi(yeartxt)), NULL);
  gtk_label_set_text(GTK_LABEL(monthlabel), setlabel);
  g_free(setlabel);
}

void on_next_button_click(GtkButton *button, GtkBuilder *builder)
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

void on_pre_button_click(GtkButton *button, GtkBuilder *builder)
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

int main(int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *window, *pre_button, *next_button;
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
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
  pre_button = GTK_WIDGET(gtk_builder_get_object(builder, "pre_button"));
  next_button = GTK_WIDGET(gtk_builder_get_object(builder, "next_button"));
  
  /* connect button actions */
  g_signal_connect(pre_button,  "clicked",
                   G_CALLBACK(on_pre_button_click),  (gpointer)builder);
  g_signal_connect(next_button, "clicked",
                   G_CALLBACK(on_next_button_click), (gpointer)builder);
  
  /* show initial calendar */
  showcal(builder);
  
  gtk_builder_connect_signals(builder, NULL);
  
  gtk_main();
  
  return 0;
}
