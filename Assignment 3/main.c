#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

GtkApplication *app;

int GetAverageFromFile(char* out){

  //Get the last line of the file
  char tmp[1024];
  FILE* fp = fopen("ping_res.txt", "r");
  if (fp == NULL) return 1;	//file not found error
  
  while(!feof(fp)) fgets(tmp, 1024, fp);
  fclose(fp);
  
  if (strncmp("rtt ", tmp, 4) != 0) return 2;  //invalid file format error
  
  //Get the substring containing the average
  char* p = strtok (tmp,"//");
  int i = 0;
  while (p != NULL && i < 4){
    p = strtok (NULL, "/");
    i++;
  }
  
  //store it in the buffer
  strcpy(out, p);
  
  return 0;
}

void showWindow(char* timeString){
  GtkWidget *window, *ipLabel;
  
  //window
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Ping Result");
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 150);
  
  //time label
  char timeDisplay[128];
  sprintf(timeDisplay, "Average Time:\n\n%s ms", timeString);
  ipLabel = gtk_label_new(timeDisplay);
  g_object_set (ipLabel, "margin", 20, NULL);
  gtk_container_add (GTK_CONTAINER (window), ipLabel);
  
  gtk_widget_show_all(window);
}

void ping(GtkWidget *pingButton, GtkWidget *ipNumbers[]){
	//Parse UI text
	char *ipPart1, *ipPart2, *ipPart3, *ipPart4, *pings;
	ipPart1 = (char*)gtk_entry_get_text(GTK_ENTRY(ipNumbers[0]));
	ipPart2 = (char*)gtk_entry_get_text(GTK_ENTRY(ipNumbers[1]));
	ipPart3 = (char*)gtk_entry_get_text(GTK_ENTRY(ipNumbers[2]));
	ipPart4 = (char*)gtk_entry_get_text(GTK_ENTRY(ipNumbers[3]));
	pings = (char*)gtk_entry_get_text(GTK_ENTRY(ipNumbers[4]));

	//Create the system ping command
	char buffer[255];
	snprintf(buffer, 255, "ping %s.%s.%s.%s -c %s | tee ping_res.txt", ipPart1, ipPart2, ipPart3, ipPart4, pings);
	printf("%s",buffer);
	system(buffer);
	
	//show the ping result window
	char buf[256];
  	if (GetAverageFromFile(buf) == 0){
  		showWindow(buf);
  	}
  	
	return;
}

static void activate(GtkApplication* app, gpointer user_data)
{

	GtkWidget *window, *grid; //layout
	GtkWidget *number1, *number2, *number3, *number4, *pingCount; //text entries
	GtkWidget *ipLabel, *pingCountLabel; //labels
	GtkWidget *pingButton; //buttons
	
	//Set up the window
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Ping Tester V1.0");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

	//this function destroys the window and exits gtk_main when the close x is pressed
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);

	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window),grid);


	ipLabel = gtk_label_new("Enter IPV4 Address");
	gtk_grid_attach(GTK_GRID(grid), ipLabel, 0, 0, 2, 1);

	//Create fields for entering IP address and attach them to Grid
	number1 = gtk_entry_new();//last four arguments are column,row, Objsize colums, Objsize rows 
	gtk_grid_attach(GTK_GRID(grid), number1, 0,3,1,1);

	number2 = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), number2, 1,3,1,1);

	number3 = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), number3, 2,3,1,1);

	number4 = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), number4, 3,3,1,1);
	

	pingCountLabel = gtk_label_new("Ping Count");
	gtk_grid_attach(GTK_GRID(grid), pingCountLabel, 0, 4, 2, 1);

	pingCount = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), pingCount, 0,5,1,1);
	
	//Set default values
	gtk_entry_set_text((GtkEntry*)number1, "192");
	gtk_entry_set_text((GtkEntry*)number2, "168");
	gtk_entry_set_text((GtkEntry*)number3, "0");
	gtk_entry_set_text((GtkEntry*)number4, "1");
	gtk_entry_set_text((GtkEntry*)pingCount, "1");
	
	//Create the pingButton
	pingButton = gtk_button_new_with_label("Ping!");

	//Use clicked signal from ping button to call ping function
	GtkWidget *ipNumbers[5] = {number1, number2, number3, number4, pingCount};
	g_signal_connect(pingButton, "clicked", G_CALLBACK(ping), ipNumbers);
	gtk_grid_attach(GTK_GRID(grid), pingButton, 2,5,2,1); //attach ping button to grid


	gtk_widget_show_all(window);
	gtk_main();
}


int main(int argc, char **argv){

	int status;
	
	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	
	return status;
}

