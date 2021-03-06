#!/usr/bin/perl

sub trim($)
{
	my $s = shift;
	$s =~ s/^\s+//;
	$s =~ s/\s+$//;
	return $s;
}

sub syntax_error($$)
{
	my $line_number = shift;
	my $message = shift;
	print "Syntax error on line ".$line_number.": ".$message."\n";
	exit;
}

sub parser_error($$)
{
	my $line_number = shift;
	my $message = shift;
	print "Parser error on line ".$line_number.": ".$message."\n";
	exit;
}

sub get_class_name($)
{
	my $class_name = trim(shift);
	$class_name =~ s/(^|\s+)(.)/\u$2/g;
	return $class_name;
}

sub get_member_name($)
{
	my $member_name = trim(shift);	
	$member_name =~ s/([a-z])([A-Z])/$1_$2/g;
	$member_name = lc($member_name);
	$member_name =~ s/\s+/_/g;
	return $member_name;
}

sub generate_struct
{
	my ($n, %defs) = @_;
	my $collector = "struct $n\n";
	$collector .= "{\n";

	foreach $var_name (sort keys %defs) 
	{
		$collector .= "\t$defs{$var_name}{type} $var_name;\n";
	}

	$collector .= "};\n\n";

	return $collector;
}

sub generate_accessor
{
	my ($n, $class_name, $control, %defs) = @_;
	my $widget = get_member_name($control);
	my $member_name = get_member_name($n);
	
	#getter
	$collector = "$n $class_name\::get_$member_name() const\n{\n";
	$collector .= "\t$n tmp;\n\n";
	foreach $var_name (sort keys %defs)
	{
		$collector .= "\ttmp.$var_name = $widget->get<$defs{$var_name}{type}>(\"$var_name\");\n";
	}

	$collector .= "\n\treturn tmp;\n}\n\n";
	
	$collector .= "void $class_name\::set_$member_name(const $n& $member_name)\n{\n";
	$collector .= "\t$widget->start_transaction();\n";
	foreach $var_name (sort keys %defs)
	{
		$collector .= "\t$widget->set<$defs{$var_name}{type}>(\"$var_name\", $member_name.$var_name);\n";
	}
	$collector .= "\t$widget->stop_transaction();\n";
	$collector .= "}\n\n";
	
	return $collector;
}

sub generate_widget_builder
{
	my ($class_name, %defs) = @_;
	
	$collector = "#include <QVBoxLayout>\n";
	$collector .= "#include <map>\n\n";
	$collector .= "void $class_name\::build_widget()\n{\n";
	$collector .= "\tstd::map<std::string, QVBoxLayout*> lay_map;\n\n";
	
	foreach $var_name (sort keys %defs)
	{
		my $tab_name = $defs{$var_name}{tab};
		my $control_object = get_member_name($defs{$var_name}{control});
		$collector .= "\tif (!lay_map.count(\"$tab_name\"))\n";
		$collector .= "\t\tlay_map[\"$tab_name\"] = new QVBoxLayout();\n\n";
		$collector .= "\tlay_map[\"$tab_name\"]->addWidget($control_object);\n";
	}
	
	$collector .= "\n\tfor (auto it = lay_map.begin(); it != lay_map.end(); ++it)\n\t{\n";
	$collector .= "\t\tQFrame* frame = new QFrame();\n";
	$collector .= "\t\tframe->setLayout(it->second);\n";
	$collector .= "\t\tmain_widget->addItem(frame, it->first.c_str());\n\t}\n";
	
	$collector .= "}\n";
	
	return $collector;
}

$arg_cnt = $#ARGV + 1;

if ($arg_cnt != 3)
{
	print "\nUsage: ap_code_generator.pl <description file> <plugin class name> <working path>\n";
	exit;
}

$path =  $ARGV[2]."/";

my $autogenerate_string = "// auto-generated file! Don't modify it!\n\n";

open my $input, "<", $path.$ARGV[0] or die $!;

$main_class_name = get_class_name($ARGV[1]);
$output_filename = "include/apconfig/".$main_class_name.".h";
my $e_output_filename = "apconfig/".$main_class_name.".h";
$cpp_output_filename = $main_class_name.".cpp";

open $output_interface, ">", $path."../core/include/core/plugin_interfaces/IApConfigPlugin.h" or die $!;
open $output, ">", $path.$output_filename or die $!;
open $output_cpp, ">", $path.$cpp_output_filename or die $!;

open $output_builder, ">", $path.$main_class_name."Builder.cpp" or die $!;

print $output_builder $autogenerate_string;
print $output_builder "#include \"$e_output_filename\"\n\n";
print $output_builder "#ifdef _WIN32\n";
print $output_builder "#define EXPORT_DEF __declspec( dllexport )\n";
print $output_builder "#else\n";
print $output_builder "#define EXPORT_DEF\n";
print $output_builder "#endif\n\n";
print $output_builder "extern \"C\" EXPORT_DEF HighFlyers::$main_class_name* factory_method()\n{\n";
print $output_builder "\treturn new HighFlyers::$main_class_name();\n}\n";

$ifdef_str = uc($output_filename);
$ifdef_str =~ s/[^a-zA-Z\d]+/_/g;

print $output $autogenerate_string;
print $output "#ifndef ".$ifdef_str."\n";
print $output "#define ".$ifdef_str."\n\n";
print $output "#include \"core/plugin_interfaces/IApConfigPlugin.h\"\n";
print $output "#include <QToolBox>\n\n";
print $output "namespace HighFlyers\n{\n";

print $output_interface "#ifndef I_AP_CONFIG_PLUGIN_H\n";
print $output_interface "#define I_AP_CONFIG_PLUGIN_H\n\n";
print $output_interface "#include \"IPlugin.h\"\n";
print $output_interface "#include <QtWidgets>\n";
print $output_interface "\nnamespace HighFlyers\n{\n";

print $output_cpp $autogenerate_string;
print $output_cpp "#include \"$e_output_filename\"\n\n";
print $output_cpp "using namespace HighFlyers;\n\n";

my $struct_begin = 0;
my $has_name = 0;
my $has_conrol = 0;
my $has_tab = 0;
my %definitions = ();
my $name;
my $control;
my $tab;
my %defined_structs = ();

while (my $line = <$input>)
{
	$line = trim($line);

	if ($line eq "{")
	{
		if ($struct_begin == 1)
		{
			syntax_error($., "unexpected '{'");
		}
		$struct_begin = 1;
	}
	elsif ($line eq "}")
	{
		if ($struct_begin == 0)
		{
			syntax_error($., "unexpected '}'");
		}
		if ($has_name == 0)
		{
			parser_error($., "`name` undefined");
		}
		if ($has_control == 0)
		{
			parser_error($., "`control` undefined");
		}
		if ($has_tab == 0)
		{
			parser_error($., "`tab` undefined");
		}
		$struct_begin = 0;
		$has_name = 0;
		$has_control = 0;
		$has_tab = 0;
		print $output_interface generate_struct($name, %definitions);
		print $output_cpp generate_accessor($name, $main_class_name, $control, %definitions);
		$defined_structs{$name} = {
			control => $control,
			tab => $tab
		};
		%definitions = ();
	}
	elsif ($line =~ m/name\s*:\s*(.+)\s*$/)
	{
		if ($has_name != 0)
		{
			parser_error($., "redefinition of `name` previously defined on line ".$has_name);
		}
		$name = get_class_name($1);
		$has_name = $.;
	}
	elsif ($line =~ m/control\s*:\s*(.+)\s*$/)
	{
		if ($has_control!= 0)
		{
			parser_error($., "redefinition of `control` previously defined on line ".$has_control);
		}
		$control = get_class_name($1);
		$has_control = $.;
	}
	elsif ($line =~ m/tab\s*:\s*(.+)\s*$/)
	{
		if ($has_tab != 0)
		{
			parser_error($., "redefinition of `tab` previously defined on line ".$has_tab);
		}
		$tab = $1;
		$has_tab = $.;
	}
	elsif ($line =~ m/(.+):(.+)$/)
	{
		my $var_name = $1;
		my $type = $2;
		my $editor = $3;
		$var_name = get_member_name($var_name);
		
		$definitions{$var_name} = {
			type => trim($type),
			editor => trim($editor)
		};
	}
}
print $output "\n} // namespace HighFlyers\n\n";

foreach $var_name (sort keys %defined_structs) 
{
	$filename = "apconfig/widgets/$defined_structs{$var_name}{control}.h";

	if (-e $path."include/".$filename)
	{
		print $output "#include \"$filename\"\n";
	}
}

print $output_interface "\nclass IApConfigPlugin : public IPlugin\n";
print $output_interface "{\n";
print $output_interface "public:\n";
print $output_interface "\tvirtual ~IApConfigPlugin(){}\n";
print $output_interface "\tvirtual QWidget* get_widget() = 0;\n";

print $output "\nnamespace HighFlyers\n{\n";
print $output "\nclass $main_class_name : public IApConfigPlugin\n";
print $output "{\n";
print $output "private:\n";
print $output "\tQToolBox* main_widget;\n";

foreach $var_name (sort keys %defined_structs) 
{
	print $output "\t$defined_structs{$var_name}{control}* ".get_member_name($defined_structs{$var_name}{control}).";\n";
}

print $output "\n\tvoid build_widget();\n";

print $output "\npublic:\n";
print $output "\tstatic PluginType get_type() { return PluginType::APCONFIG; }\n";
print $output "\tPluginType get_type_t() { return get_type(); }\n";
print $output "\tQWidget* get_widget() { return main_widget; }\n";

#constructor generator
print $output "\t$main_class_name();\n";
print $output_cpp "$main_class_name\::$main_class_name()\n{";
print $output_cpp "\n\tmain_widget = new QToolBox();\n";

foreach $var_name (sort keys %defined_structs)
{
	my $member_name = get_member_name($defined_structs{$var_name}{control});
	print $output_cpp "\n\t$member_name = new $defined_structs{$var_name}{control}();";
}

print $output_cpp "\n\n\tbuild_widget();";
print $output_cpp "\n}\n\n";


foreach $var_name (sort keys %defined_structs)
{
	my $member_name = get_member_name($var_name);
	print $output "\t$var_name get_$member_name() const;\n";
	print $output "\tvoid set_$member_name(const $var_name& $member_name);\n";
	print $output_interface "\tvirtual $var_name get_$member_name() const = 0;\n";
	print $output_interface "\tvirtual void set_$member_name(const $var_name& $member_name) = 0;\n";
}

print $output "};\n";
print $output "\n}\n";
print $output "\n#endif\n";

print $output_interface "};\n";
print $output_interface "\n}\n";
print $output_interface "\n#endif\n";

print $output_cpp generate_widget_builder($main_class_name, %defined_structs);

