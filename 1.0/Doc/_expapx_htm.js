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
'expapx.htm',
'expapxseq.htm',
'expapxfor.htm',
'expapxrev.htm',
'expapxad.cpp.htm',
'introduction.cpp.htm'
];
var list_down0 = [
'expapx.hpp.htm',
'expapx.cpp.htm'
];
var list_current0 = [
'expapx.htm#Syntax',
'expapx.htm#Purpose',
'expapx.htm#Mathematical Form',
'expapx.htm#include',
'expapx.htm#x',
'expapx.htm#e',
'expapx.htm#y',
'expapx.htm#Type',
'expapx.htm#Implementation',
'expapx.htm#Example'
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
