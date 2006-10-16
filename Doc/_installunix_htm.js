var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_down2 = [
'license.htm',
'faq.htm',
'install.htm',
'getstarted.cpp.htm',
'introduction.htm',
'ad.htm',
'adfun.htm',
'library.htm',
'example.htm',
'appendix.htm'
];
var list_down1 = [
'installunix.htm'
];
var list_current0 = [
'installunix.htm#Download Current Version',
'installunix.htm#User Documentation',
'installunix.htm#Developer Documentation',
'installunix.htm#Configure',
'installunix.htm#PrefixDir and PostfixDir',
'installunix.htm#--with-GetStarted',
'installunix.htm#--with-Introduction',
'installunix.htm#--with-Example',
'installunix.htm#--with-TestMore',
'installunix.htm#--with-Speed',
'installunix.htm#--with-PrintFor',
'installunix.htm#--with-SpeedExample',
'installunix.htm#--with-profiling',
'installunix.htm#--with-stdvector',
'installunix.htm#AdolcDir',
'installunix.htm#FadbadDir',
'installunix.htm#BoostDir',
'installunix.htm#CppErrorsAndWarnings',
'installunix.htm#make',
'installunix.htm#Final Installation'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
