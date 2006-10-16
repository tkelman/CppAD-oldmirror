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
'constructor.htm',
'convert.htm',
'eq.htm',
'advalued.htm',
'boolvalued.htm',
'vecad.htm'
];
var list_down2 = [
'arithmetic.htm',
'stdmathunary.htm',
'mathother.htm',
'condexp.htm',
'discrete.htm'
];
var list_down1 = [
'unaryplus.htm',
'unaryminus.htm',
'add.htm',
'sub.htm',
'mul.htm',
'div.htm',
'addeq.htm',
'subeq.htm',
'muleq.htm',
'diveq.htm'
];
var list_down0 = [
'addeq.cpp.htm'
];
var list_current0 = [
'addeq.htm#Syntax',
'addeq.htm#Purpose',
'addeq.htm#x',
'addeq.htm#y',
'addeq.htm#Result',
'addeq.htm#Operation Sequence',
'addeq.htm#Derivative',
'addeq.htm#Example'
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
