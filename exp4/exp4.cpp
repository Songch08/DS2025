#define NOMINMAX
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <chrono>
#include <windows.h>
#include <iomanip>
using namespace std;
using namespace std::chrono;
struct Box {
    int x, y, w, h;
    float confidence;
};
template <typename T>
int partition(vector<T>& arr, int low, int high) {
    T pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] > pivot) { 
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}
template <typename T>
void quicksort(vector<T>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}
template <typename Iter>
void merge_sort_impl(Iter first, Iter last) {
    auto n = distance(first, last);
    if (n <= 1) return;
    auto mid = first + n / 2;
    merge_sort_impl(first, mid);
    merge_sort_impl(mid, last);
    vector<typename iterator_traits<Iter>::value_type> temp;
    temp.reserve(n);
    merge(first, mid, mid, last, back_inserter(temp), 
          [](const auto& a, const auto& b) { return a > b; }); 
    copy(temp.begin(), temp.end(), first);
}
template <typename T>
void merge_sort(vector<T>& arr) {
    merge_sort_impl(arr.begin(), arr.end());
}
template <typename T>
void bubble_sort(vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) { 
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}
template <typename T>
void insertion_sort(vector<T>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] < key) { 
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
float calculate_iou(Box& box1, Box& box2) {
    int x1 = max(box1.x, box2.x);
    int y1 = max(box1.y, box2.y);
    int x2 = min(box1.x + box1.w, box2.x + box2.w);
    int y2 = min(box1.y + box1.h, box2.y + box2.h);
    int inter_w = max(0, x2 - x1);
    int inter_h = max(0, y2 - y1);
    int inter_area = inter_w * inter_h;
    int area1 = box1.w * box1.h;
    int area2 = box2.w * box2.h;
    return static_cast<float>(inter_area) / (area1 + area2 - inter_area);
}
vector<Box> efficient_nms(vector<Box> boxes, float iou_threshold = 0.5) {
    if (boxes.empty()) return {};
    vector<Box> result;
    vector<bool> suppressed(boxes.size(), false);
    sort(boxes.begin(), boxes.end(), 
        [](const Box& a, const Box& b) { 
            return a.confidence > b.confidence; 
        });
    for (size_t i = 0; i < boxes.size(); ++i) {
        if (suppressed[i]) continue;
        result.push_back(boxes[i]);
        for (size_t j = i + 1; j < boxes.size(); ++j) {
            if (suppressed[j]) continue;
            float iou = calculate_iou(boxes[i], boxes[j]);
            if (iou > iou_threshold) {
                suppressed[j] = true;
            }
        }
    }
    return result;
}
vector<Box> generate_data(int num_boxes, bool clustered = false) {
    const int MAX_COORD = 1000;
    const int MAX_SIZE = 100;
    const int MIN_SIZE = 20;
    vector<Box> boxes;
    boxes.reserve(num_boxes);
    vector<pair<int, int>> cluster_centers;
    if (clustered) {
        int num_clusters = max(1, num_boxes / 50);
        for (int i = 0; i < num_clusters; i++) {
            cluster_centers.push_back({
                rand() % MAX_COORD,
                rand() % MAX_COORD
            });
        }
    }
    for (int i = 0; i < num_boxes; ++i) {
        Box box;
        if (clustered) {
            int cluster_idx = rand() % cluster_centers.size();
            auto& center = cluster_centers[cluster_idx];
            box.x = center.first + rand() % 100 - 50;
            box.y = center.second + rand() % 100 - 50;
        } else {
            box.x = rand() % MAX_COORD;
            box.y = rand() % MAX_COORD;
        }
        box.w = rand() % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
        box.h = rand() % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
        box.confidence = static_cast<float>(rand()) / RAND_MAX;
        box.x = max(0, min(MAX_COORD - box.w, box.x));
        box.y = max(0, min(MAX_COORD - box.h, box.y));
        boxes.push_back(box);
    }
    return boxes;
}
void run_performance_tests() {
    srand(static_cast<unsigned>(time(nullptr)));
    const vector<int> sizes = {100, 500, 1000, 2000, 5000, 10000};
    const vector<string> dist_names = {"随机分布", "聚集分布"};
    for (int size : sizes) {
        for (int dist = 0; dist <= 1; dist++) {
            string dist_name = dist_names[dist];
            vector<Box> boxes = generate_data(size, dist == 1);
            vector<float> confidences;
            for (const auto& box : boxes) {
                confidences.push_back(box.confidence);
            }
            cout << "\n数据分布: " << dist_name << ", 数据量: " << size << "\n";
            cout << "---------------------------------------------------------------\n";
            auto data = confidences;
            auto start = high_resolution_clock::now();
            quicksort(data, 0, data.size() - 1);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start).count();
            cout << "快速排序时间: " << duration << " μs\n";
            data = confidences;
            start = high_resolution_clock::now();
            merge_sort(data);
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start).count();
            cout << "归并排序时间: " << duration << " μs\n";
            if (size <= 5000) {
                data = confidences;
                start = high_resolution_clock::now();
                bubble_sort(data);
                end = high_resolution_clock::now();
                duration = duration_cast<microseconds>(end - start).count();
                cout << "起泡排序时间: " << duration << " μs\n";
            } else {
                cout << "起泡排序: 未测试（数据量过大）\n";
            }
            if (size <= 5000) {
                data = confidences;
                start = high_resolution_clock::now();
                insertion_sort(data);
                end = high_resolution_clock::now();
                duration = duration_cast<microseconds>(end - start).count();
                cout << "插入排序时间: " << duration << " μs\n";
            } else {
                cout << "插入排序: 未测试（数据量过大）\n";
            }
            data = confidences;
            start = high_resolution_clock::now();
            sort(data.begin(), data.end(), greater<float>());
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start).count();
            cout << "std::sort时间: " << duration << " μs\n";
            auto boxes_copy = boxes;
            start = high_resolution_clock::now();
            auto result = efficient_nms(boxes_copy);
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start).count();
            cout << "NMS执行时间: " << duration << " μs\n";
        }
    }
    cout << "===============================================================\n";
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    run_performance_tests();
    return 0;
}