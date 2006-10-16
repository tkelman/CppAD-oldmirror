var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_down4 = [
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
var list_down3 = [
'independent.htm',
'funconstruct.htm',
'dependent.htm',
'seqproperty.htm',
'funeval.htm',
'drivers.htm',
'funcheck.htm',
'fundeprecated.htm'
];
var list_down2 = [
'forward.htm',
'reverse.htm',
'sparse.htm'
];
var list_down1 = [
'reverseone.htm',
'reverseany.htm'
];
var list_down0 = [
'reverseone.cpp.htm'
];
var list_current0 = [
'reverseone.htm#Syntax',
'reverseone.htm#Purpose',
'reverseone.htm#f',
'reverseone.htm#x',
'reverseone.htm#w',
'reverseone.htm#dw',
'reverseone.htm#Vector',
'reverseone.htm#Example'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_down4(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down4[index-1];
}
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
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
