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
'arithmetic.xml',
'stdmathunary.xml',
'mathother.xml',
'condexp.xml',
'discrete.xml'
];
var list_down0 = [
'acos.cpp.xml',
'asin.cpp.xml',
'atan.cpp.xml',
'cos.cpp.xml',
'cosh.cpp.xml',
'exp.cpp.xml',
'log.cpp.xml',
'log10.cpp.xml',
'sin.cpp.xml',
'sinh.cpp.xml',
'sqrt.cpp.xml',
'tan.cpp.xml'
];
var list_current0 = [
'stdmathunary.xml#Syntax',
'stdmathunary.xml#Purpose',
'stdmathunary.xml#x',
'stdmathunary.xml#y',
'stdmathunary.xml#Operation Sequence',
'stdmathunary.xml#fun',
'stdmathunary.xml#Examples',
'stdmathunary.xml#Derivatives',
'stdmathunary.xml#Derivatives.acos',
'stdmathunary.xml#Derivatives.asin',
'stdmathunary.xml#Derivatives.atan',
'stdmathunary.xml#Derivatives.cos',
'stdmathunary.xml#Derivatives.cosh',
'stdmathunary.xml#Derivatives.exp',
'stdmathunary.xml#Derivatives.log',
'stdmathunary.xml#Derivatives.log10',
'stdmathunary.xml#Derivatives.sin',
'stdmathunary.xml#Derivatives.sinh',
'stdmathunary.xml#Derivatives.sqrt',
'stdmathunary.xml#Derivatives.tan'
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
