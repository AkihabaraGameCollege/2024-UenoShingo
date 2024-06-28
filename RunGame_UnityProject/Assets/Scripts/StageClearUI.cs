using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;

// ステージクリアーUIの進行制御を管理します。
public class StageClearUI : MonoBehaviour
{
    // 「NEXT」ボタンが押されたときに発生する UnityEvent
    public UnityEvent onNextButtonClick;
    // 「NEXT」ボタンを指定します。
    [SerializeField]
    private Button nextButton = null;

    void Awake()
    {
        // UnityEvent を追加
        nextButton.onClick.AddListener(OnNextButtonClick);
    }

    // Start is called before the first frame update
    void Start()
    {
        Hide();
    }

    // 「NEXT」ボタンが押されたときに呼び出されます。
    private void OnNextButtonClick()
    {
        onNextButtonClick.Invoke();
    }

    // このUIを表示します。
    public void Show()
    {
        // すべての子オブジェクトをアクティブに設定
        foreach (Transform child in this.transform)
        {
            child.gameObject.SetActive(true);
        }
        nextButton.Select();
    }

    // このUIを隠します。
    public void Hide()
    {
        // すべての子オブジェクトを非アクティブに設定
        foreach (Transform child in this.transform)
        {
            child.gameObject.SetActive(false);
        }
    }
}
