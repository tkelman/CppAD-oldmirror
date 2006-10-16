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
'constructor.htm',
'convert.htm',
'eq.htm',
'advalued.htm',
'boolvalued.htm',
'vecad.htm'
];
var list_down1 = [
'arithmetic.htm',
'stdmathunary.htm',
'mathother.htm',
'condexp.htm',
'discrete.htm'
];
var list_down0 = [
'acos.cpp.htm',
'asin.cpp.htm',
'atan.cpp.htm',
'cos.cpp.htm',
'cosh.cpp.htm',
'exp.cpp.htm',
'log.cpp.htm',
'log10.cpp.htm',
'sin.cpp.htm',
'sinh.cpp.htm',
'sqrt.cpp.htm',
'tan.cpp.htm'
];
var list_current0 = [
'stdmathunary.htm#Syntax',
'stdmathunary.htm#Purpose',
'stdmathunary.htm#x',
'stdmathunary.htm#y',
'stdmathunary.htm#Operation Sequence',
'stdmathunary.htm#fun',
'stdmathunary.htm#Examples',
'stdmathunary.htm#Derivatives',
'stdmathunary.htm#Derivatives.acos',
'stdmathunary.htm#Derivatives.asin',
'stdmathunary.htm#Derivatives.atan',
'stdmathunary.htm#Derivatives.cos',
'stdmathunary.htm#Derivatives.cosh',
'stdmathunary.htm#Derivatives.exp',
'stdmathunary.htm#Derivatives.log',
'stdmathunary.htm#Derivatives.log10',
'stdmathunary.htm#Derivatives.sin',
'stdmathunary.htm#Derivatives.sinh',
'stdmathunary.htm#Derivatives.sqrt',
'stdmathunary.htm#Derivatives.tan'
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
