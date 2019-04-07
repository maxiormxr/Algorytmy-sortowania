#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace std;

void generuj_pseudolosowe(long long a, long long *tab);
void wprowadz_liczby(long long a, long long *tab);
void sort_wstaw(long long a, long long &b, long long *tab);
void sort_szybkie(long long l, long long p, long long *tab, long long &t);
void wypisz(long long a, long long *tab);
void h_przywracanie(long long *tab, long long rozmiar, long long i, long long &t, bool &flag, int &najwiekszy);
void h_budowa(long long *tab, long long rozmiar, long long &t, bool &flag, int &najwiekszy);
void sort_kopcowanie(long long *tab, long long rozmiar, long long &t, bool &flag, int &najwiekszy);
void scalanie(long long *tab, long long *tab_pom, int lewy, int srodek, int prawy, long long &t);
void sort_scalanie(long long *tab, long long *tab_pom, int lewy, int prawy, long long &t);
int podzial(long long *tab, long long l, long long r, long long &t);
int podzial_los(long long *tab, long long l, long long r, long long &t);
int wybor(long long *tab, long long l, long long r, long long k, long long &t);

int main()
{
	long long n, t = 0, przelacznik, powtorz_program, ilosc_wyswietlonych, wybrana = 0;
	clock_t start, stop;
	double czas;
	bool przywrac_flag;
	int najwiekszy;
	int kk;

	do
	{
		cout << "PODAJ ILOSC LICZB DO SORTOWANIA: ";
		cin >> n;
		cout << endl;

		long long *tablica, *w;
		tablica = new long long[n + 1];
		w = tablica;

		cout << "WYBIERZ SPOSOB WPROWADZANIA LICZB" << endl << "1. WPISZ CIAG Z KLAWIATURYY" << endl << "2. WYGENERUJ CIAG" << endl;

		cin >> przelacznik;
		switch (przelacznik)
		{
		case 1:
			wprowadz_liczby(n, tablica);
			break;
		case 2:
			generuj_pseudolosowe(n, tablica);
			break;
		default:
			cout << "NIEPOPRAWNY WYBOR" << endl;
			break;
		}
		cout << "WYBIERZ SPOSOB SORTOWANIA: " << endl << "1. SORTOWANIE PRZEZ WSTAWIANIE" << endl << "2. SORTOWANIE SZYBKIE" << endl << "3. SORTOWANIE PRZEZ KOPCOWANIE" << endl << "4. SORTOWANIE PRZEZ SCALANIE" << endl << "5. WYBOR WG ALGORYTMU HOARE'A" << endl;
		cin >> przelacznik;
		switch (przelacznik)
		{
		case 1:
			start = clock();
			sort_wstaw(n, t, tablica);
			stop = clock();
			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			break;
		case 2:
			start = clock();
			sort_szybkie(0, (n - 1), tablica, t);
			stop = clock();
			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			break;
		case 3:
			start = clock();
			sort_kopcowanie(tablica, n, t, przywrac_flag, najwiekszy);
			stop = clock();
			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			break;
		case 4:
			long long *tab_pomocnicza;
			tab_pomocnicza = new long long[n + 1];
			start = clock();
			sort_scalanie(tablica, tab_pomocnicza, 0, n - 1, t);
			stop = clock();
			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			delete[] tab_pomocnicza;
			break;
		case 5:
			long long k, j;
			cout << "WYBIERZ ELEMENT DO WYDRUKOWANIA (ROSNACO): " << endl;
			cin >> k;
			start = clock();
			wybrana = wybor(tablica, 1, n, k, t);
			stop = clock();
			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			kk = k;
			break;
		default:
			cout << "NIEPOPRAWNY WYBOR" << endl;
			break;
		}
		if (przelacznik != 5)
		{
			cout << "ILE POCZATKOWYCH LICZB PO SORTOWANIU WYSWIETLIC: " << endl;
			cin >> ilosc_wyswietlonych;
			wypisz(ilosc_wyswietlonych, tablica);
		}
		if (przelacznik == 5)
		{
			cout << kk << " ELEMENT (ROSNACO): " << wybrana << endl;
		}

		delete[] tablica;

		if (przelacznik == 1 || przelacznik == 2) cout << "liczba wykonanych porownan t: " << t << endl << "c = " << double(t) / (n * n) << endl;
		else if (przelacznik != 5) cout << "liczba wykonanych porownan t: " << t << endl << "c = " << double(t) / (n * log(n)) << endl;
		else if (przelacznik == 5) cout << "liczba wykonanych porownan t: " << t << endl << "c = " << double(t) / (n) << endl;
		cout << "czas wykonywania: ";
		cout.precision(2); 
		cout << czas;
		cout << " sekund" << endl;

		t = 0;

		cout << "WYKONAC TEST PONOWNIE?" << endl << "1. TAK" << endl << "2. NIE" << endl;
		cin >> powtorz_program;
	} while (powtorz_program == 1);
}
void generuj_pseudolosowe(long long a, long long *tab)
{
	int irnd;
	srand(time(NULL));
	tab++;
	for (int i = 1; i <= a; i++)
	{
		irnd = rand() % 100000;
		*tab = irnd;
		if (i <= 100)cout << "liczba " << i << ": " << *tab << endl;
		tab++;	
	}
	cout << endl;
}
void wprowadz_liczby(long long a, long long *tab)
{
	tab++;
	for (int i = 1; i <= a; i++)
	{
		cout << "wprowadz " << i << " liczbe: ";
		cin >> *tab;
		tab++;
	}
	cout << endl;
}
void sort_wstaw(long long a, long long &b, long long *tab)
{
	int j, tt = 0;
	for (int i = 2; i <= a; i++)
	{
		tab[0] = tab[i];
		j = i - 1;
		while (j >= 0 && tab[0] < tab[j])
		{
			tab[j + 1] = tab[j];
			j--;
			tt++;
		}
		tab[j + 1] = tab[0];
	}
	b = tt;
}
void sort_szybkie(long long l, long long p, long long *tab, long long &t)
{
	int i = 1, j, temp;
	i = (l + p) / 2;
	temp = tab[i];
	tab[i] = tab[p];
	for (j = i = l; i < p; i++)
		if (tab[i] < temp)
		{
			t++;
			swap(tab[i], tab[j]);
			j++;
		}
	tab[p] = tab[j]; tab[j] = temp;
	if (l < j - 1)  sort_szybkie(l, j - 1, tab, t);
	if (j + 1 < p) sort_szybkie(j + 1, p, tab, t);
}
void wypisz(long long a, long long *tab)
{
	for (int i = 1; i <= a; i++)
	{
		cout << "liczba " << i << ": " << tab[i] << endl;
	}
}
void h_przywracanie(long long *tab, long long rozmiar, long long i, long long &t, bool &flag, int &najwiekszy)
{
	do
	{
		int temp;
		int l = 2 * i, r = (2 * i) + 1;
		if (l <= rozmiar && tab[l] > tab[i]) najwiekszy = l;
		else najwiekszy = i;
		t++;
		if (r <= rozmiar && tab[r] > tab[najwiekszy]) najwiekszy = r;
		t++;
		if (najwiekszy != i)
		{
			temp = tab[najwiekszy];
			tab[najwiekszy] = tab[i];
			tab[i] = temp;
			flag = true;
			i = najwiekszy;
		}
		else flag = false;
	}
	while (flag == true);
}
void h_budowa(long long *tab, long long rozmiar, long long &t, bool &flag, int &najwiekszy)
{
	for (int i = rozmiar / 2; i>0; i--)
		h_przywracanie(tab, rozmiar, i, t, flag, najwiekszy);
}
void sort_kopcowanie(long long *tab, long long rozmiar, long long &t, bool &flag, int &najwiekszy)
{

	int temp;
	h_budowa(tab, rozmiar, t, flag, najwiekszy);
	for (int i = rozmiar; i>1; i--)
	{
		temp = tab[i];
		tab[i] = tab[1];
		tab[1] = temp;
		rozmiar--;
		h_przywracanie(tab, rozmiar, 1, t, flag, najwiekszy);
	}
}
void scalanie(long long *tab, long long *tab_pom, int lewy, int srodek, int prawy, long long &t)
{
	int i = lewy, j = srodek + 1;

	for (int i = lewy; i <= prawy; i++)
		tab_pom[i] = tab[i];

	for (int k = lewy; k <= prawy; k++)
		if (i <= srodek)
			if (j <= prawy)
				if (tab_pom[j] < tab_pom[i])
				{
					tab[k] = tab_pom[j++];
					t++;
				}
				else
				{
					tab[k] = tab_pom[i++];
					t++;
				}

			else
			{
				tab[k] = tab_pom[i++];
				t++;
			}
		else
		{
			tab[k] = tab_pom[j++]; 
			t++;
		}	
}
void sort_scalanie(long long *tab, long long *tab_pom, int lewy, int prawy, long long &t)
{
	if (prawy <= lewy) return;
	int srodek = (prawy + lewy) / 2;
	sort_scalanie(tab, tab_pom, lewy, srodek, t);
	sort_scalanie(tab, tab_pom, srodek + 1, prawy, t);
	scalanie(tab, tab_pom, lewy, srodek, prawy, t);
}
int podzial(long long *tab, long long l, long long r, long long &t)
{
	int i, j;
	long long p = tab[l];
	i = l - 1;
	j = r + 1;
	while (i < j)
	{
		while (tab[j] > p) 
		{ 
			j = j - 1;
			t++;
		}
		while (tab[i] < p)
		{ 
			i = i + 1; 
			t++;
		}
		if (i < j) swap(tab[i], tab[j]);
	}
	return j;
}
int podzial_los(long long *tab, long long l, long long r, long long &t)
{
	int irnd, j = 0;
	srand(time(NULL));
	irnd = (rand() % r) + 1;
	swap(tab[l], tab[irnd]);
	j = podzial(tab, l, r, t);
	return j;
}
int wybor(long long *tab, long long l, long long r, long long k, long long &t)
{
	long long m, j;
	if (l < r)
	{
		j = podzial_los(tab, l, r, t);
		m = j - l + 1;
		if (k <= m) return wybor(tab, l, j, k, t);
		else return wybor(tab, j + 1, r, k - m, t);
	}
	else return tab[l];
}