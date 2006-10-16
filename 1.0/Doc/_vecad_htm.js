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
'constructor.htm',
'convert.htm',
'eq.htm',
'advalued.htm',
'boolvalued.htm',
'vecad.htm'
];
var list_down0 = [
'vecad.cpp.htm'
];
var list_current0 = [
'vecad.htm#Syntax',
'vecad.htm#Purpose',
'vecad.htm#Alternatives',
'vecad.htm#VecAD&lt;Base&gt;::reference',
'vecad.htm#VecAD&lt;Base&gt;::reference.Exceptions',
'vecad.htm#n',
'vecad.htm#v',
'vecad.htm#size',
'vecad.htm#i',
'vecad.htm#b',
'vecad.htm#x',
'vecad.htm#y',
'vecad.htm#Example',
'vecad.htm#Speed and Memory'
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
