#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#ifdef DEBUG
typedef struct
{
	GstDebugCategory* category;
	GstDebugLevel level;
	const gchar* file;
	const gchar* fnc;
	gint line;
	GObject* object;
	GstDebugMessage* message;
} DebugInfo;
#endif

#endif // DEBUGINFO_H
