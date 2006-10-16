var list_across0 = [
'_contents_xml.htm',
'_reference.xml',
'_index.xml',
'_search_xml.htm',
'_external.xml'
];
var list_down4 = [
'license.xml',
'faq.xml',
'install.xml',
'getstarted.cpp.xml',
'introduction.xml',
'ad.xml',
'adfun.xml',
'library.xml',
'example.xml',
'appendix.xml'
];
var list_down3 = [
'constructor.xml',
'convert.xml',
'eq.xml',
'advalued.xml',
'boolvalued.xml',
'vecad.xml'
];
var list_down2 = [
'arithmetic.xml',
'stdmathunary.xml',
'mathother.xml',
'condexp.xml',
'discrete.xml'
];
var list_down1 = [
'abs.xml',
'atan2.xml',
'erf.xml',
'pow.xml'
];
var list_down0 = [
'atan2.cpp.xml'
];
var list_current0 = [
'atan2.xml#Syntax',
'atan2.xml#Purpose',
'atan2.xml#Base',
'atan2.xml#y',
'atan2.xml#x',
'atan2.xml#theta',
'atan2.xml#Operation Sequence',
'atan2.xml#Example'
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
