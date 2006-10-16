var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_down3 = [
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
var list_down2 = [
'general.htm',
'exampleutility.htm',
'listallexamples.htm',
'cppadvector.htm'
];
var list_down1 = [
'interface2c.cpp.htm',
'jacminordet.cpp.htm',
'jacludet.cpp.htm',
'hesminordet.cpp.htm',
'hesludet.cpp.htm',
'odestiff.cpp.htm',
'odetaylor.cpp.htm',
'complexpoly.cpp.htm',
'multape.htm'
];
var list_down0 = [
'multape.cpp.htm'
];
var list_current0 = [
'multape.htm#Background',
'multape.htm#Motivation',
'multape.htm#General Solution',
'multape.htm#General Procedure',
'multape.htm#General Procedure.Start ADBaseTape',
'multape.htm#General Procedure.Start ADDBaseTape',
'multape.htm#General Procedure.Inner Function Calculations',
'multape.htm#General Procedure.Derivative of Inner Function',
'multape.htm#General Procedure.Outer Function',
'multape.htm#General Procedure.Derivative of Outer Function',
'multape.htm#Example'
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
