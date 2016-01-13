#include "pch.h"
#include "MainPage.xaml.h"
#include "BitSet.h"
#include "RingBuffer.h"
#include <string>
#include <ppltasks.h>

using namespace PowerBall;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;
using namespace std;

MainPage::MainPage()
{
	InitializeComponent();
}

void PowerBall::MainPage::RandomizeButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    const auto ws = wstring(MainPage::InputTextBox->Text->Data());
	uint32_t user_seed_modifier = 0;
	if (ws.length() > 0)
	{
		user_seed_modifier = stoul(ws.substr(0,9));
	}

	MainPage::RandomizeButton->IsEnabled = false;

	// https://msdn.microsoft.com/en-us/magazine/hh781020.aspx
	// create_async https://msdn.microsoft.com/en-us/library/hh750102.aspx
	// lambda function https://msdn.microsoft.com/en-us/library/dd293608.aspx
	//Windows::Foundation::IAsyncAction^
	const auto action = concurrency::create_async([this, user_seed_modifier]()
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		LARGE_INTEGER freq;
		LARGE_INTEGER t1;
		LARGE_INTEGER t2;
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&t1);

		uint32_t seed = user_seed_modifier ^ ((st.wMilliseconds ^ st.wSecond) + (st.wMinute << st.wHour));
		auto rng = mt19937(seed);

		auto uni = uniform_int_distribution<size_t>(1, 69);
		auto uni2 = uniform_int_distribution<size_t>(1, 26);
		auto uni3 = uniform_int_distribution<size_t>(512, 2048);
		
		BitSet b = BitSet(69); // bitset for number selection
		BitSet b2 = BitSet(26); // bitset for powerball selection
		size_t r1 = uni3(rng); // number of numbers to burn before step 1
		
		size_t r = uni(rng);
		size_t j = 0;

		// Burn in 1
		for (j = 0; j < r1; ++j)
		{
			r = uni3(rng);
		}

		QueryPerformanceCounter(&t2);

		uint32_t seed2 = seed ^ (uint32_t)t2.QuadPart; // new seed from 
		auto rng2 = mt19937(seed2);
		size_t r2 = uni3(rng2); // number of numbers to burn before step 2

		// Random number selection
		uint64_t i = 0;
		while (b.available > 0)
		{
			r = uni(rng2);
			b.set(r);
			++i;
		}

		// Burn in 2
		for (j = 0; j < r2; ++j)
		{
			r = uni2(rng2);
		}

		// Random powerball selection
		i = 0;
		while (b2.available > 0)
		{
			r = uni2(rng2);
			b2.set(r);
			++i;
		}

		// Collect numbers
		size_t num1 = b.pop();
		size_t num2 = b.pop();
		size_t num3 = b.pop();
		size_t num4 = b.pop();
		size_t num5 = b.pop();
		
		// Collect powerball number
		size_t powerball = b2.pop();

		// Convert to String
		const std::wstring sp = std::wstring(L" ");
		const std::wstring ws = std::to_wstring(num1) + sp +
			std::to_wstring(num2) + sp +
			std::to_wstring(num3) + sp +
			std::to_wstring(num4) + sp +
			std::to_wstring(num5) + sp +
			std::to_wstring(powerball);
		const wchar_t* wc = ws.c_str();
		Platform::String^ ps = ref new Platform::String(wc);
		return ps;
	});
	
	// Collect result and update ResultsTextBox
	Concurrency::create_task(action).then([this](Platform::String^ ps) {
		MainPage::ResultsTextBox->Text = ps;
		MainPage::RandomizeButton->IsEnabled = true;
	});
	
}
