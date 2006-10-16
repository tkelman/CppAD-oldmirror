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
'theory.htm',
'speed.htm',
'adolc.htm',
'fadbad.htm',
'glossary.htm',
'bib.htm',
'bugs.htm',
'wishlist.htm',
'whatsnew06.htm'
];
var list_down1 = [
'whatsnew05.htm',
'whatsnew04.htm',
'whatsnew03.htm'
];
var list_current0 = [
'whatsnew03.htm#Introduction',
'whatsnew03.htm#12-24',
'whatsnew03.htm#12-22',
'whatsnew03.htm#12-14',
'whatsnew03.htm#12-13',
'whatsnew03.htm#12-12',
'whatsnew03.htm#12-10',
'whatsnew03.htm#12-05',
'whatsnew03.htm#12-01',
'whatsnew03.htm#11-21',
'whatsnew03.htm#11-20',
'whatsnew03.htm#11-16',
'whatsnew03.htm#11-15',
'whatsnew03.htm#11-14',
'whatsnew03.htm#11-12',
'whatsnew03.htm#11-11',
'whatsnew03.htm#11-06',
'whatsnew03.htm#11-04',
'whatsnew03.htm#11-02',
'whatsnew03.htm#10-21',
'whatsnew03.htm#10-16',
'whatsnew03.htm#10-14',
'whatsnew03.htm#10-10',
'whatsnew03.htm#10-06',
'whatsnew03.htm#10-05',
'whatsnew03.htm#09-30',
'whatsnew03.htm#09-20',
'whatsnew03.htm#09-19',
'whatsnew03.htm#09-18',
'whatsnew03.htm#09-15',
'whatsnew03.htm#09-14',
'whatsnew03.htm#09-13',
'whatsnew03.htm#09-06',
'whatsnew03.htm#09-05',
'whatsnew03.htm#09-04',
'whatsnew03.htm#09-03',
'whatsnew03.htm#08-23',
'whatsnew03.htm#08-22',
'whatsnew03.htm#08-19',
'whatsnew03.htm#08-17',
'whatsnew03.htm#08-16',
'whatsnew03.htm#08-11',
'whatsnew03.htm#08-10',
'whatsnew03.htm#08-07',
'whatsnew03.htm#08-06',
'whatsnew03.htm#08-05',
'whatsnew03.htm#08-04',
'whatsnew03.htm#08-03',
'whatsnew03.htm#08-01',
'whatsnew03.htm#07-30',
'whatsnew03.htm#07-29',
'whatsnew03.htm#07-26',
'whatsnew03.htm#07-22',
'whatsnew03.htm#07-20',
'whatsnew03.htm#07-18',
'whatsnew03.htm#07-16',
'whatsnew03.htm#07-14'
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
