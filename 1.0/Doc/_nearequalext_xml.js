var list_across0 = [
'_contents_xml.htm',
'_reference.xml',
'_index.xml',
'_search_xml.htm',
'_external.xml'
];
var list_down3 = [
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
var list_down2 = [
'constructor.xml',
'convert.xml',
'eq.xml',
'advalued.xml',
'boolvalued.xml',
'vecad.xml'
];
var list_down1 = [
'compare.xml',
'nearequalext.xml',
'boolfun.xml',
'parvar.xml'
];
var list_down0 = [
'nearequalext.cpp.xml'
];
var list_current0 = [
'nearequalext.xml#Syntax',
'nearequalext.xml#Purpose',
'nearequalext.xml#x',
'nearequalext.xml#y',
'nearequalext.xml#r',
'nearequalext.xml#a',
'nearequalext.xml#b',
'nearequalext.xml#Type',
'nearequalext.xml#Operation Sequence',
'nearequalext.xml#Example'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
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
