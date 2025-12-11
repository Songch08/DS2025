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

// 边界框结构体
struct Box {
    int x, y, w, h;
    float confidence;
};

// 1. 排序算法实现（降序排列）

// 快速排序
template <typename T>
int partition(vector<T>& arr, int low, int high) {
    T pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] > pivot) { // 降序排序
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

// 归并排序
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
          [](const auto& a, const auto& b) { return a > b; }); // 降序合并
    
    copy(temp.begin(), temp.end(), first);
}

template <typename T>
void merge_sort(vector<T>& arr) {
    merge_sort_impl(arr.begin(), arr.end());
}

// 起泡排序
template <typename T>
void bubble_sort(vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) { // 降序排序
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// 插入排序
template <typename T>
void insertion_sort(vector<T>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] < key) { // 降序排序
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 2. NMS算法实现
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
    
    // 按置信度降序排序
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

// 3. 数据生成器
vector<Box> generate_data(int num_boxes, bool clustered = false) {
    const int MAX_COORD = 1000;
    const int MAX_SIZE = 100;
    const int MIN_SIZE = 20;
    
    vector<Box> boxes;
    boxes.reserve(num_boxes);
    
    // 为聚集分布创建聚类中心
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
            // 在聚类中心周围生成边界框
            int cluster_idx = rand() % cluster_centers.size();
            auto& center = cluster_centers[cluster_idx];
            
            box.x = center.first + rand() % 100 - 50;
            box.y = center.second + rand() % 100 - 50;
        } else {
            // 随机分布
            box.x = rand() % MAX_COORD;
            box.y = rand() % MAX_COORD;
        }
        
        box.w = rand() % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
        box.h = rand() % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
        box.confidence = static_cast<float>(rand()) / RAND_MAX;
        
        // 确保边界框在有效范围内
        box.x = max(0, min(MAX_COORD - box.w, box.x));
        box.y = max(0, min(MAX_COORD - box.h, box.y));
        
        boxes.push_back(box);
    }
    return boxes;
}

// 4. 性能测试框架
void run_performance_tests() {
    srand(static_cast<unsigned>(time(nullptr)));
    
    // 测试不同数据规模
    const vector<int> sizes = {100, 500, 1000, 2000, 5000, 10000};
    const vector<string> dist_names = {"随机分布", "聚集分布"};
    
    cout << "===============================================================\n";
    cout << "排序算法与NMS性能测试\n";
    cout << "===============================================================\n";
    
    // 对每种数据规模和分布进行测试
    for (int size : sizes) {
        for (int dist = 0; dist <= 1; dist++) {
            string dist_name = dist_names[dist];
            
            // 生成测试数据
            vector<Box> boxes = generate_data(size, dist == 1);
            vector<float> confidences;
            for (const auto& box : boxes) {
                confidences.push_back(box.confidence);
            }
            
            // 输出当前测试信息
            cout << "\n数据分布: " << dist_name << ", 数据量: " << size << "\n";
            cout << "---------------------------------------------------------------\n";
            
            // 测试快速排序
            auto data = confidences;
            auto start = high_resolution_clock::now();
            quicksort(data, 0, data.size() - 1);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start).count();
            cout << "快速排序时间: " << duration << " μs\n";
            
            // 测试归并排序
            data = confidences;
            start = high_resolution_clock::now();
            merge_sort(data);
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start).count();
            cout << "归并排序时间: " << duration << " μs\n";
            
            // 测试起泡排序（仅在小数据量时测试）
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
            
            // 测试插入排序（仅在小数据量时测试）
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
            
            // 测试std::sort
            data = confidences;
            start = high_resolution_clock::now();
            sort(data.begin(), data.end(), greater<float>());
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start).count();
            cout << "std::sort时间: " << duration << " μs\n";
            
            // 测试NMS
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