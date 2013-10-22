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

$arg_cnt = $#ARGV + 1;

if ($arg_cnt != 2)
{
	print "\nUsage: ap_code_generator.pl <description file> <plugin class name>\n";
	exit;
}

open my $input, "<", $ARGV[0] or die $!;

$main_class_name = get_class_name($ARGV[1]);
$output_filename = $main_class_name.".h";
$cpp_output_filename = $main_class_name.".cpp";

open $output, ">", $output_filename or die $!;
open $output_cpp, ">", $cpp_output_filename or die $!;

$ifdef_str = uc($output_filename);
$ifdef_str =~ s/[^a-zA-Z\d]+/_/g;

print $output "#ifndef ".$ifdef_str."\n";
print $output "#define ".$ifdef_str."\n\n";

print $output_cpp "// auto-generated file! Don't modify it!\n\n";
print $output_cpp "#include \"$output_filename\"\n\n";

my $struct_begin = 0;
my $has_name = 0;
my $has_conrol = 0;
my %definitions = ();
my $name;
my $control;
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
		$struct_begin = 0;
		$has_name = 0;
		$has_control = 0;
		print $output generate_struct($name, %definitions);
		$defined_structs{$name} = $control;
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

print $output "class $main_class_name\n";
print $output "{\n";
print $output "private:\n";


foreach $var_name (sort keys %defined_structs) 
{
	print $output "\t$var_name ".get_member_name($var_name).";\n";
	print $output "\t$defined_structs{$var_name}* ".get_member_name($defined_structs{$var_name}).";\n";
}

print $output "\npublic:\n";

#constructor generator
print $output "\t$main_class_name();\n";
print $output_cpp "$main_class_name\::$main_class_name()\n{";

foreach $var_name (sort keys %defined_structs)
{
	my $member_name = get_member_name($defined_structs{$var_name});
	print $output_cpp "\n\t$member_name = new $defined_structs{$var_name}();";
}
print $output_cpp "\n}\n\n";

#destructor generator
print $output "\tvirtual ~$main_class_name();\n";
print $output_cpp "$main_class_name\::~$main_class_name()\n{";

foreach $var_name (sort keys %defined_structs)
{
	my $member_name = get_member_name($defined_structs{$var_name});
	print $output_cpp "\n\tdelete $member_name;";
}
print $output_cpp "\n}\n\n";

foreach $var_name (sort keys %defined_structs)
{
	my $member_name = get_member_name($var_name);
	print $output "\t$var_name get_$member_name() const;\n";
	print $output "\tvoid set_$member_name(const $var_name& $member_name);\n";
	print $output_cpp "$var_name $main_class_name\::get_$member_name() const\n";
	print $output_cpp "{\n\treturn $member_name;\n}\n\n";
	print $output_cpp "void $main_class_name\::set_$member_name(const $var_name& $member_name)\n";
	print $output_cpp "{\n\tthis->$member_name = $member_name;\n}\n\n";	
}

print $output "};\n";

print $output "\n#endif\n";
